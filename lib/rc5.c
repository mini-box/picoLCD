#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rc5.h"

#ifdef __MACOS__
#include <sys/time.h>

#define CLOCK_METHOD 0

/* MacOS has no clock_gettime, quick function to replace but loses precision */
int clock_gettime(int method, struct timespec *tp)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tp->tv_sec = tv.tv_sec;
    tp->tv_nsec = tv.tv_usec*1000;
    return 0;
}
#else

#define CLOCK_METHOD CLOCK_MONOTONIC    
#endif

#define PRECISION 1000000000

//#define DEBUG
#undef DEBUG

static struct timespec rc5_get_delay(struct timespec start, struct timespec end)
{
        struct timespec elapsed;
        
        if ((end.tv_nsec - start.tv_nsec) < 0) { 
                elapsed.tv_sec = end.tv_sec - start.tv_sec + 1; 
                elapsed.tv_nsec = PRECISION + end.tv_nsec - start.tv_nsec; 
        } else { 
                elapsed.tv_sec = end.tv_sec - start.tv_sec; 
                elapsed.tv_nsec = end.tv_nsec - start.tv_nsec; 
        }
#ifdef DEBUG        
        fprintf(stderr, "Start seconds: %d nsec: %d, End seconds: %d nsec: %d, Elapsed seconds: %d nsec: %d\n", 
    			start.tv_sec, start.tv_nsec,
    			end.tv_sec, end.tv_nsec,
                        elapsed.tv_sec, elapsed.tv_nsec); 
#endif

        return elapsed;
}

static void rc5_save_delay(rc5decoder *rc5, struct timespec delay)
{
    rc5->command_delay.tv_sec = delay.tv_sec;
    rc5->command_delay.tv_nsec = delay.tv_nsec;
}

static void rc5_addbit(rc5decoder *rc5, char bit)
{
	rc5->bits[rc5->nbits] = bit;
	rc5->nbits++;
	
	return;
}

static int rc5_checkcode(rc5decoder *rc5, short int code)
{
	int   codetype = 0;

	code = -code; /* negating the code (complementary) */

	if (code < 0)
	{/* space */
		short abscode = abs(code);

		if ( (abscode > 444 ) && (abscode < 1333) )
			 codetype = RC5_SHORT_SPACE;
		else if ( (abscode > 1334 ) && (abscode < 2222) )
			 codetype = RC5_LONG_SPACE;
	}
	else
	{ /* pulse */
		if ( (code > 444 ) && (code < 1333) )
			 codetype = RC5_SHORT_PULSE;
		else if ( (code > 1334 ) && (code < 2222) )
			 codetype = RC5_LONG_PULSE;
	}

	if (codetype == 0) return 0;

	if (rc5->state == RC5_STATE_BEGIN)
	{
		rc5->state = RC5_STATE_MID1;
		rc5_addbit(rc5, 1);
#ifdef DEBUG		
		printf("_|");
#endif
	}
#ifdef DEBUG
	switch (codetype)
	{
	case RC5_SHORT_PULSE:	printf("~|");break;
	case RC5_LONG_PULSE:	printf("~~|");break;
	case RC5_SHORT_SPACE:	printf("_|");break;
	case RC5_LONG_SPACE:	printf("___|");break;
	}
#endif
	switch (rc5->state)
	{
	case RC5_STATE_BEGIN:
		rc5->state = RC5_STATE_MID1;
		rc5_addbit(rc5, 1);
		break;

	case RC5_STATE_MID0:
		switch (codetype)
		{
		case RC5_SHORT_SPACE:
			rc5->state = RC5_STATE_START0;
			break;
		case RC5_LONG_SPACE:
			rc5->state = RC5_STATE_MID1;
			rc5_addbit(rc5, 1);
			break;
		default:
			printf("<RC5_STATE_MID0 Err (%d)>\n",codetype);
			return 0;
		}
		break;

	case RC5_STATE_MID1:
		switch (codetype)
		{
		case RC5_SHORT_PULSE:
			rc5->state = RC5_STATE_START1;
			break;
		case RC5_LONG_PULSE:
			rc5->state = RC5_STATE_MID0;
			rc5_addbit(rc5, 0);
			break;
		default:
			printf("<RC5_STATE_MID1 Err (%d)>\n",codetype);
			return 0;
		}
		break;

	case RC5_STATE_START0:
		switch (codetype)
		{
		case RC5_SHORT_PULSE:
			rc5->state = RC5_STATE_MID0;
			rc5_addbit(rc5, 0);
			break;
		default:
			printf("<RC5_STATE_START0 Err (%d)>\n",codetype);
			return 0;
		}
		break;

	case RC5_STATE_START1:
		switch (codetype)
		{
		case RC5_SHORT_SPACE:
			rc5->state = RC5_STATE_MID1;
			rc5_addbit(rc5, 1);
			break;
		default:
			printf("<RC5_STATE_START1 Err (%d)>\n",codetype);
			return 0;
		}
		break;
	
	default:
		return 0;
	}

	return 1;
}


int rc5_decode(rc5decoder *rc5, unsigned char *data, int len)
{
    int i = 0;
    
    struct timespec delay, elapsed;
    
    if (rc5 == NULL) {
	fprintf(stderr, "rc5 decoder not initialized !\n");
	return 0;
    }
    
    clock_gettime(CLOCK_METHOD, &delay);
    elapsed = rc5_get_delay(rc5->command_delay, delay);
    
    if (elapsed.tv_nsec / 1000 / 1000 > RC5_BUTTON_DELAY)
    {
		printf("IR_RC5: New command\n");
		rc5_close(rc5);
		rc5 = rc5_init();
    }
    
    rc5_save_delay(rc5, delay);

    if ((len % 2)!=0)
    {
	printf("IR_RC5: Bad command\n");
	rc5_close(rc5);
	rc5 = rc5_init();
	return 0;
    }

    //convert the buffer in short's (every pulse is on 2 bytes)
    short* buff_short = (short*)data;

    for (i = 0; i < (len/2); i++)
    {
	if (! rc5_checkcode(rc5, buff_short[i]))
	{
		printf("IR_RC5: Bad pulse (%d=%d %x)\n",i,buff_short[i],buff_short[i]&0xFFFF);
		rc5_close(rc5);
		rc5 = rc5_init();
	}
	else
	{
		//printf("IR_RC5: OK pulse (%d=%d %x)\n",i,buff_short[i],buff_short[i]&0xFFFF);
		if (rc5->nbits >= 14)
		{
			int b;
			printf("\nIR_RC5: COMMAND [");
			for (b = 0; b < 14; b++)
				printf("%d",rc5->bits[b]);
				printf("]\n");
				rc5_close(rc5);
				rc5 = rc5_init();
			}
		}
	}
	printf("\n");

    return 1;
}

rc5decoder * rc5_init(void)
{
    rc5decoder *rc5;
    
    if ((rc5 = (rc5decoder *) malloc (sizeof(rc5decoder))) == NULL)
	return NULL;
    
    rc5->state = RC5_STATE_BEGIN;
    rc5->nbits = 0;
    memset(rc5->bits, 0, RC5_COMMAND_LEN);
    clock_gettime(CLOCK_METHOD, &rc5->command_delay);
    
    return rc5;
}

void rc5_close(rc5decoder *rc5)
{
    if (rc5 != NULL)
	free(rc5);
    
    return;
}

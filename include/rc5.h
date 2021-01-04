#ifndef __RC5_H__
#define __RC5_H__

#define RC5_SHORT_PULSE		1
#define RC5_LONG_PULSE		2
#define RC5_SHORT_SPACE		3
#define RC5_LONG_SPACE		4

#define RC5_STATE_BEGIN		0
#define RC5_STATE_MID0		1
#define RC5_STATE_MID1		2
#define RC5_STATE_START0	3
#define RC5_STATE_START1	4

#define RC5_COMMAND_LEN 	14
#define RC5_BUTTON_DELAY	60
/* 
rc5 decoded command:
    2 start bits
    1 toggle bit
    5 address bits
    6 command bits
*/

typedef struct _rc5decoder_s rc5decoder;
struct _rc5decoder_s {
    int state;
    int nbits;
    char bits[RC5_COMMAND_LEN]; /* a rc5 decoded command */
    struct timespec command_delay; /* keeps track of delays between rc5 commands */
};

rc5decoder * rc5_init(void);
int rc5_decode(rc5decoder *rc5, unsigned char *data, int len);
void rc5_close(rc5decoder *rc5);
#endif


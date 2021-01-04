/* Copyright 2006 iTuner Corporation */
/* npavel@ituner.com */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

#include "picolcd.h"
#include "rc5.h"
#include "widgets.h"
#include "picolcd-util.h"

//#define DEBUG
#undef DEBUG

void usage (char *program)
{
    printf ("Usage: %s [commands]\n", program);
    printf ("Commands and their parameters should be delimeted by space.\n");
    printf ("-------------------------------------------------------------------\n");
    printf ("Command list:\n");
    printf ("\n");
    printf (" backlight [0|1]\n");
    printf ("\tturns on or off LCD screen backlight\n");
    printf (" contrast [0-40]\n");
    printf ("\tset the contrast. 0 is maximum contrast and 40 minimum\n");
    printf (" led [led number] [0|1]\n");
    printf ("\tturn on/off one of the front panel leds (1-8)\n");
    printf ("\n");
    printf (" clear\n");
    printf ("\tclears LCD screen\n");
    printf (" text [row:0-1][column:0-19][text]\n");
    printf ("\tprints text on LCD screen starting with row and column\n");
    printf (" splash [filename]\n");
    printf ("\tsets the splash screens content from filename.\n");
    printf (" setfont [filename]\n");
    printf ("\toverwrite the first 10 characters from LCD CGRAM\n");
    printf ("\twith the caracters from filename.\n");
    printf (" flash [filename] - write the firmware from filename into EEPROM\n");
    printf (" read\n");
    printf ("\tread events (key press and infrared data) and outputs to stderr\n");
    printf (" wait [seconds]\n");
    printf ("\twait for a number of seconds before executing the next command.\n");
    printf ("\n");
    printf ("Example: %s led 1 1 wait 1 led 2 1 wait 1 backlight 1 contrast 0\n", program);
    printf (" text 1 2 \"mini-box.com USB LCD\" read\n");
    printf ("This will lightup first led then wait 1 second, light the second\n");
    printf ("led and wait 1 second, will turn backlight on, set the contrast \n");
    printf ("to maximum, write the \"mini-box.com USB LCD\" text to second row\n");
    printf ("starting from third column, and will continue printing events\n");
    printf ("till closed with CTRL+C or SIGTERM\n");
    printf ("-------------------------------------------------------------------\n");
    printf ("%s is based on libpicolcd copyright 2006-2010 mini-box.com.\n", program);
}

int main (int argc, char **argv)
{
    /* for connecting and communication with the device */
    usblcd_operations *mylcd;
    /* for keypad and infrared events */
    usblcd_event *event;
    
    char *s;
    FILE *file;
    int value, nvalue;
    int arg = 1;

    if (argc <= 1) {
        usage(argv[0]);
        return 0;
    }
    
    /* init hid device and usblcd_operations structure */
    if ((mylcd = new_driver(PICOLCD20x4)) == NULL)
	return 1;
	
    /* set hid debug level */
    mylcd->hid->debug(5);
    /* set usblcd debug level */
    mylcd->debug(5);
    /* init the USB LCD */
    mylcd->init(mylcd);
    /* clear the LCD screen */
    //mylcd->clear(mylcd);
    /* clear all leds status */
    mylcd->setled(mylcd, 0, 0);
    /* set default cursor */
    mylcd->set_cursor(mylcd, 0);    
    /* set default non blink cursor */
    mylcd->set_cursor_blink(mylcd, 0);    
    
    /* process arguments and send commands to the LCD */
    while ( arg < argc ) {
	s = argv[arg];	
	
	if (strncmp(s, "backlight", 9) == 0) {
	    if (is_number(argv[arg+1])) {
		value = atoi(argv[arg+1]);
		mylcd->backlight(mylcd, value);
		arg++;
	    }
	}
	
	if (strncmp(s, "contrast", 8) == 0) {
	    if (is_number(argv[arg+1])) {
		value = atoi(argv[arg+1]);
		mylcd->contrast(mylcd, value);
		arg++;
	    }
	}
	
	if (strncmp(s, "text", 4) == 0) {
	    value = nvalue = 0;
	    if ( arg + 2 < argc && is_number(argv[arg+1])) {
		value = atoi(argv[arg+1]);
		arg++;
		
		if (arg + 2 < argc && is_number(argv[arg+1])) {
		    nvalue = atoi(argv[arg+1]);
		    arg++;
		}
	    } 
	    if (arg + 1 < argc) {
		mylcd->settext(mylcd, value, nvalue, argv[arg+1]);
		arg++;
	    }
	}
	
	if (strncmp(s, "led", 3) == 0) {
	    value = nvalue = 0;
	    if (is_number(argv[arg+1])) {
		value = atoi(argv[arg+1]);
		arg++;
		if (is_number(argv[arg+1])) {
		    nvalue = atoi(argv[arg+1]);
		    arg++;
		}
		
	    }
	    mylcd->setled(mylcd, value, nvalue);
	}
	
	if (strncmp(s, "wait", 4) == 0) {
	    value = 1;
	    if (is_number(argv[arg+1])) {
		value = atoi(argv[arg+1]);
		arg++;
	    }
	    usleep (value * 100000);
	}
	
	if (strncmp(s, "setfont", 7) == 0) {
	    if ((file = fopen(argv[arg+1], "r")) != NULL) {
		mylcd->setfont(mylcd, argv[arg+1]);
		arg++;
	    } 
	    else  
		printf("File %s not readable\n",argv[arg+1]);
	}
	
	if (strncmp(s, "splash", 6) == 0) {
	    if ((file = fopen(argv[arg+1], "r")) != NULL) {
		mylcd->setsplash(mylcd, argv[arg+1]);
		arg++;
	    } 
	    else  
		printf("File %s not readable\n",argv[arg+1]);
	}
	
	if (strncmp(s, "flash", 5) == 0) {
	    if ((file = fopen(argv[arg+1], "r")) != NULL) {
		mylcd->flash(mylcd);
		arg++;
	    } 
	    else  
		printf("File %s not readable\n",argv[arg+1]);
	}
	
	if (strncmp(s, "clear", 5) == 0) {
		mylcd->clear(mylcd);
	}
	
	if (strncmp(s, "read", 4) == 0) {
	    rc5decoder *rc5;
	    rc5 = rc5_init();
	    
	    while (1) 
	    {
		
		if ((event = mylcd->read_events(mylcd)) == NULL)
		    continue;
		
		if (event->type == 0) 
		{
		    fprintf(stderr, "Key: %02x %02x\n", event->data[0], event->data[1]);
		}
		
		if (event->type == 1)
		{
#ifdef DEBUG		    
		    int i;
		    for (i = 0; i < event->length; i++)
			fprintf(stderr, "%02x ", event->data[i]);
		    printf("\n");
#endif
		    
		    rc5_decode(rc5, event->data, event->length);
		}
		
		usleep(10);
		
		free(event->data);
		free(event);
	    }
	}	
	
	if (strncmp(s, "histo", 5) == 0) {
	    histo_opts options;
	    int values[64],i;
	    srand((unsigned)time(0));
 
	    // left to right histo with text
	    options.width = 20;
	    options.height = 2;
	    options.orientation = 0;  
	    options.valuetype = 0;
	    options.barwidth = 5;
	    options.barspacing = 0;
	    for (i=0;i<64;i++) values[i] = (int)(16*(rand()/(RAND_MAX+1.0)));
	    histo(mylcd, 0, 0, options, 20, values);

	}
	
	if (strncmp(s, "smallhisto", 10) == 0) {
	    histo_opts options;
	    int values[64],i;
	    srand((unsigned)time(0));
 
	    // left to right histo with text
	    options.width = 4;
	    options.height = 2;
	    options.orientation = 0;  
	    options.valuetype = 0;
	    options.barwidth = 1;
	    options.barspacing = 0;
	    for (i=0;i<64;i++) values[i] = (int)(16*(rand()/(RAND_MAX+1.0)));
	    histo(mylcd, 0, 0, options, 20, values);
	}
	
	arg++;
    }

//    mylcd->setchar(mylcd, 0, 1, 255);    
    
/*
    mylcd->setfont(mylcd,"../doc/examples/bigfonts/mail.fnt");
    mylcd->setchar(mylcd, 0, 0, 0); 
    mylcd->setchar(mylcd, 1, 0, 1); 
    
    mylcd->setchar(mylcd, 0, 1, 2); 
    mylcd->setchar(mylcd, 1, 1, 3); 
    
    mylcd->setchar(mylcd, 0, 2, 4); 
    mylcd->setchar(mylcd, 1, 2, 5); 
    
    mylcd->setchar(mylcd, 0, 3, 6); 
    mylcd->setchar(mylcd, 1, 3, 7); 
*/

    mylcd->close(mylcd);
    return 0;
}

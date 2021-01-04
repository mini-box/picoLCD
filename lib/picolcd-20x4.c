/* Copyright 2006-2008 iTuner Corporation */
/* npavel@ituner.com */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "driver.h"
#include "picolcd.h"
#include "picolcd-common.h"
#include "debug.h"
#include "util.h"

/* doesn't support specifing a column, pad with spaces */
void picolcd_20x4_settext(usblcd_operations *self, unsigned int row, unsigned int column, char *text)
{

    char packet[64];
        
    char lineset[4][6] = {
	{ 0x94, 0x00, 0x01, 0x00, 0x64, 0x80 },
	{ 0x94, 0x00, 0x01, 0x00, 0x64, 0xC0 },
        { 0x94, 0x00, 0x01, 0x00, 0x64, 0x94 },
        { 0x94, 0x00, 0x01, 0x00, 0x64, 0xD4 }
    };
        
    int len, i;
    	
    switch(row) {
	case 0: picolcd_send(self, lineset[0], 6);  break;
        case 1: picolcd_send(self, lineset[1], 6);  break;
        case 2: picolcd_send(self, lineset[2], 6);  break;
        case 3: picolcd_send(self, lineset[3], 6);  break;
        default: picolcd_send(self, lineset[0], 6);  break;
    }
    
    len = strlen(text);
    if (len > 20) len = 20;

    packet[0] = 0x95;
    packet[1] = 0x01;
    packet[2] = 0x00;
    packet[3] = 0x01;
    packet[4] = len;

    i = 5;
    while (len--) 
    {
            packet[i++] = *text++;
    }

    picolcd_send(self, packet, i);
}

static usblcd_operations driver_20x4 = {

    .state 			= NULL,
    .hid			= NULL,
    .vendorid			= 0x04d8,
    .productid			= 0xc001,
    .rows			= 3,
    .cols			= 19,
    .max_data_len		= 24,
    .max_leds			= 8,
    .max_splashes		= 5,
    .max_custom_chars		= 8,
    .custom_char_height		= 8,
    .custom_char_width		= 5,
    .leds 			= 0,
    .match			= picolcd_match,
    .init 			= picolcd_init,
    .debug 			= picolcd_debug,
    .setled 			= picolcd_setled,
    .backlight 			= picolcd_backlight,
    .contrast 			= picolcd_contrast,
    .set_cursor 		= picolcd_set_cursor,
    .set_cursor_blink 		= picolcd_set_cursor_blink,
    .set_switch 		= picolcd_set_switch,
    .enter_flasher_mode 	= picolcd_enter_flasher_mode,
    .exit_flasher_mode 		= picolcd_exit_flasher_mode,
    .control 			= picolcd_control,
    .clear 			= picolcd_clear,
    .setchar			= (void *) picolcd_unimplemented,
    .settext 			= picolcd_20x4_settext,
    .setfont 			= picolcd_setfont,
    .setfontmemory		= picolcd_setfont_memory,
    .setsplash 			= picolcd_setsplash,
    .getversion 		= picolcd_getversion,
    .flash 			= picolcd_flash,
    .powerstate 		= picolcd_powerstate,
    .read_events 		= picolcd_read_events,
    .keystate 			= picolcd_keystate,
    .irdata 			= picolcd_irdata,
    .send			= picolcd_send,
    .close 			= picolcd_close,
};

usblcd_operations * driver_20x4_export(void) {

    driver_20x4.state = (usblcd_state *) malloc(sizeof(usblcd_state));
    
    driver_20x4.state->usblcd_switch = _USBLCD_SWITCH_ON;
    driver_20x4.state->usblcd_cursor = 0;
    driver_20x4.state->usblcd_cursor_blink = 0;
    
    driver_20x4.hid = new_hid_operations(driver_20x4.vendorid, driver_20x4.productid);
        
    printf("Device [0x%04x: 0x%04x]\n",  driver_20x4.hid->hiddev->vendorid, 
	     driver_20x4.hid->hiddev->productid);
    
    return &driver_20x4;
}


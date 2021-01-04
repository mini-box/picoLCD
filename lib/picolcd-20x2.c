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

static usblcd_operations driver_20x2 = {

    .state 			= NULL,
    .hid			= NULL,
    .vendorid			= 0x04d8,
    .productid			= 0x002,
    .rows			= 1,
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
    .setchar			= picolcd_setchar,
    .settext 			= picolcd_settext,
    .setfont 			= picolcd_setfont,
    .setfontmemory		= picolcd_setfont_memory,
    .setsplash 			= picolcd_setsplash,
    .getversion 		= picolcd_getversion,
    .flash 			= picolcd_flash,
    .powerstate 		= picolcd_powerstate,
    .read_events 		= picolcd_read_events,
    .keystate 			= picolcd_keystate,
    .irdata 			= picolcd_irdata,
    .close 			= picolcd_close,
};

usblcd_operations * driver_20x2_export(void) {

    driver_20x2.state = (usblcd_state *) malloc(sizeof(usblcd_state));
    
    driver_20x2.state->usblcd_switch = _USBLCD_SWITCH_ON;
    driver_20x2.state->usblcd_cursor = 0;
    driver_20x2.state->usblcd_cursor_blink = 0;
    
    driver_20x2.hid = new_hid_operations(driver_20x2.vendorid, driver_20x2.productid);
    
    if (driver_20x2.hid == NULL)
	return NULL;
    
    MESSAGE("Device [0x%04x: 0x%04x]\n",driver_20x2.vendorid, driver_20x2.productid);
    
    return &driver_20x2;
}


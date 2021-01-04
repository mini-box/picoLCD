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

static usblcd_operations driver_256x64 = {

    .state 			= NULL,
    .hid			= NULL,
    .vendorid			= 0x04d8,
    .productid			= 0xc002,
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
    .set_cursor 		= (void *) picolcd_unimplemented,
    .set_cursor_blink 		= (void *) picolcd_unimplemented,
    .set_switch 		= (void *) picolcd_unimplemented,
    .enter_flasher_mode 	= (void *) picolcd_unimplemented,
    .exit_flasher_mode 		= (void *) picolcd_unimplemented,
    .control 			= (void *) picolcd_unimplemented,
    .clear 			= (void *) picolcd_unimplemented,
    .setchar			= (void *) picolcd_unimplemented,
    .settext 			= (void *) picolcd_unimplemented,
    .setfont 			= (void *) picolcd_unimplemented,
    .setfontmemory		= (void *) picolcd_unimplemented,
    .setsplash 			= (void *) picolcd_unimplemented,
    .getversion 		= picolcd_getversion,
    .flash 			= picolcd_flash,
    .powerstate 		= picolcd_powerstate,
    .read_events 		= picolcd_read_events,
    .keystate 			= picolcd_keystate,
    .irdata 			= picolcd_irdata,
    .close 			= picolcd_close,
};

usblcd_operations * driver_256x64_export(void) {

    driver_256x64.state = (usblcd_state *) malloc(sizeof(usblcd_state));
    
    driver_256x64.state->usblcd_switch = _USBLCD_SWITCH_ON;
    driver_256x64.state->usblcd_cursor = 0;
    driver_256x64.state->usblcd_cursor_blink = 0;
    
    driver_256x64.hid = new_hid_operations(driver_256x64.vendorid, driver_256x64.productid);
        
    MESSAGE("Device [0x%04x: 0x%04x]\n", driver_256x64.vendorid, driver_256x64.productid);
    
    return &driver_256x64;
}


/* Copyright 2006-2010 iTuner Corporation */
/* Common routines between picolcd 20x2, 20x4, 256x64 */
/* npavel@ituner.com */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "driver.h"
#include "picolcd.h"
#include "debug.h"
#include "util.h"

/* public functions */

void picolcd_match(void);

void picolcd_init(usblcd_operations *self);
void picolcd_debug(int level);
void picolcd_setled(usblcd_operations *self, unsigned int led, unsigned int status);
void picolcd_backlight(usblcd_operations *self, unsigned int status);
void picolcd_contrast(usblcd_operations *self, unsigned int level);
void picolcd_set_cursor(usblcd_operations *self, unsigned int status);
void picolcd_set_cursor_blink(usblcd_operations *self, unsigned int status);
void picolcd_set_switch(usblcd_operations *self, unsigned int status);
void picolcd_control(usblcd_operations *self);
void picolcd_clear(usblcd_operations *self);
void picolcd_setchar(usblcd_operations *self, unsigned int row, unsigned int column, char character);
void picolcd_settext(usblcd_operations *self, unsigned int row, unsigned int column, char *text);
void picolcd_getversion(usblcd_operations *self);
void picolcd_enter_flasher_mode(usblcd_operations *self);
void picolcd_exit_flasher_mode(usblcd_operations *self);
void picolcd_setfont(usblcd_operations *self, char *filename);
void picolcd_setfont_memory(usblcd_operations *self, int fontlines[], int nrchars);
void picolcd_setsplash(usblcd_operations *self, char *filename);
void picolcd_flash(usblcd_operations *self);
void picolcd_keystate(usblcd_operations *self);
void picolcd_irdata(usblcd_operations *self);
void picolcd_powerstate(usblcd_operations *self);
void picolcd_send(usblcd_operations *self, char *data, int size);
usblcd_event * picolcd_read_events(usblcd_operations *self);
void picolcd_close(usblcd_operations *self);
void picolcd_unimplemented(void);
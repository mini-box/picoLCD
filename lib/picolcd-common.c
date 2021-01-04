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

/* private functions */
static usblcd_font_data * get_font_data(char *fontfile, unsigned int max_fonts, unsigned int font_width, unsigned int font_height)
{
    FILE *file;
    usblcd_font_data *chars, *font;
    char *line;
    int index = 0;
    int i = 0;
    unsigned int found = 0;
    
    if ((file = fopen(fontfile, "r")) == NULL)
	return NULL;
	
    line = (char *) malloc (255);
    if (line == NULL)
	return NULL;
    
    chars = (usblcd_font_data *) malloc (max_fonts * sizeof(usblcd_font_data));
    if (chars == NULL)
	return NULL;

    font = chars;
    
    while (index < max_fonts) 
    {
	for (i = 0; i < font_height; i++) 
	{
	    font->line[i] = (char *) malloc (font_width + 1);
	    memset(font->line[i],'0', font_width);
	}
	font->id = 0;
	index++;
	font++;
    }
    
    while(fgets(line, 255, file))
    {
	if (line[0] == '#')
	    continue;
	if (strncmp(line, FONT_HEADER, 5) == 0) {
	    found = 1;
	    break;
	}
    }
    
    if (!found) {
	fprintf(stderr,"Can't find font file header for file: %s.\n", fontfile);
	return NULL;
    }
    
    font = chars;

    while (fgets(line, 255, file)) 
    {
	if ((line[0] == '#') || (line[0]=='\n') || (line[0]==' '))
	    continue;
	font->id = atoi(line);
	for (i = 0; i < font_height ; i++) {
	    if (!fgets(line, 255, file)) 
		break;
	    snprintf(font->line[i], font_width + 1, "%s",line);
	}
	font++;
    }
    
    return chars;
}

static usblcd_splash_data * get_splash_data(char *splashfile, unsigned int max_splashes, unsigned int rows, unsigned int cols)
{
    FILE *file;
    usblcd_splash_data *splashes, *splash;
    char *line;
    char *tmp;
    int index = 0;
    int i;
    unsigned int found = 0;
    
    
    if ((file = fopen(splashfile, "r")) == NULL) 
	return NULL;
    
    line = (char *) malloc (255);
    if (line == NULL)
	return NULL;

    tmp = (char *) malloc (3);
    if (tmp == NULL)
	return NULL;
	
    splashes = (usblcd_splash_data *) malloc (max_splashes * sizeof(usblcd_splash_data));
    if (splashes == NULL)
	return NULL;
	
    
    splash = splashes;
    
    while (index < max_splashes) {
	for (i = 0; i < rows + 1; i++) {
	    splash->line[i] = (char *) malloc (cols + 1 + 1);
	    memset(splash->line[i],' ', cols + 1);
	}
	splash->min = 0;
	splash->sec = 0;
	splash->jump = 0;
	splash->repeat = 0;
	splash->leds = 0;
	splash++;
	index++;
    }
    
    while(fgets(line, 255, file))
    {
	if (line[0] == '#')
	    continue;
	if (strncmp(line, SPLASH_HEADER, 5) == 0) {
	    found = 1;
	    break;
	}
    }
    
    if (!found) {
	fprintf(stderr,"Can't find splash file header for file: %s.\n", splashfile);
	return NULL;
    }
    
    splash = splashes;
    while (fgets(line, 255, file)) 
    {
	if ((line[0] == '#') || (line[0]=='\n'))
	    continue;
	/* minutes and seconds */	
	memcpy(tmp, &line[0], 2);
	splash->min = atoi(tmp);
	if (splash->min > 59) splash->min = 59;
	
	memcpy(tmp, &line[3], 2);
	splash->sec = atoi(tmp);
	if (splash->sec > 59) splash->sec = 59;
	
	/* next slot jump and repeat */	
	fgets(line, 255, file);
	memcpy(tmp, &line[0], 2);
	splash->jump = atoi(tmp);
	if (splash->jump > 10) splash->jump = 10;
	
	memcpy(tmp, &line[3], 3);
	splash->repeat = atoi(tmp);
	if (splash->repeat > 255) splash->repeat = 255;

	/* leds state (8 chars) */	
	fgets(line, 255, file);
	splash->leds =  bin2int(line);
	
	/* lcd lines text */
	for (i = 0; i < rows + 1; i++) {
	    fgets(line, 255, file);
	    snprintf(splash->line[i], cols + 1 + 1, "%s", line);
	}
	splash++;
    }
    
    free(line);
    free(tmp);
    
    return splashes;
}

/* public functions */


void picolcd_unimplemented(void)
{
    printf("Function not implemented \n");
    return;
}


void picolcd_match(void) 
{
    
}

void picolcd_send(usblcd_operations *self, char *data, int size)
{
    hid_params *params;
    
    if ((params = (hid_params *) malloc (sizeof(hid_params))) == NULL)
	return;
    
    params->endpoint = USB_ENDPOINT_OUT + 1;
    params->timeout = HID_TIMEOUT;
    params->packetlen = size;
    params->packet = data;
    
    self->hid->interrupt_write(self->hid->hiddev->handle, params);
    
    free(params);
    return;
}


void picolcd_init(usblcd_operations *self) 
{
    /* no longer needed */
    #ifdef USB_DEV_TEST
    char const SETUP_PACKET_1[] = { 0x14, 0x03, 0x41, 0x00, 0x43, 0x00, 0x4d, 0x00 };
    char const SETUP_PACKET_2[] = { 0x20, 0x03, 0x41, 0x00, 0x43, 0x00, 0x4d, 0x00 };
    char read_packet[255];
    hid_params *params;
    
    params =(hid_params *) malloc (sizeof(hid_params));
    params->endpoint = USB_ENDPOINT_OUT + 1;
    params->packetlen = 8;
    params->timeout = HID_TIMEOUT;
    params->packet =(char *) SETUP_PACKET_1;
    self->hid->interrupt_write(self->hid->hid_id, params);
    params->packet = (char *)SETUP_PACKET_2;
    self->hid->interrupt_write(self->hid->hid_id, params);
    #endif
    
    self->getversion(self);
}

void picolcd_debug(int level) 
{
    debug_level = level;
}

void picolcd_setled(usblcd_operations *self, unsigned int led, unsigned int status) 
{
    char packet[3];
    
    if (led > self->max_leds) led = self->max_leds;
    if (status > 1 || status < 0) status = 0;
    
    /* set led bit to 1 or 0 */
    if (status)	self->leds |= 1 << led;
    else self->leds &= ~ (1 << led);

    snprintf(packet, 3 , "%c%c", OUT_REPORT_LED_STATE, self->leds);
    picolcd_send(self, packet, 2);
}

void picolcd_backlight(usblcd_operations *self, unsigned int status) 
{
    char packet[3];
    
    snprintf(packet, 2 + 1 , "%c%c", OUT_REPORT_LCD_BACKLIGHT, status);
    picolcd_send(self, packet, 2);
}

void picolcd_contrast(usblcd_operations *self, unsigned int level)
{
    char packet[3];
    snprintf (packet, 2 + 1 , "%c%c", OUT_REPORT_LCD_CONTRAST,  level);
    picolcd_send(self, packet, 2);
}

void picolcd_set_cursor(usblcd_operations *self, unsigned int status)
{
    if (status) self->state->usblcd_cursor = _USBLCD_CURSOR_ON;
    else self->state->usblcd_cursor = 0;
    self->control(self);    
}

void picolcd_set_cursor_blink(usblcd_operations *self, unsigned int status)
{
    if (status) self->state->usblcd_cursor_blink = _USBLCD_CURSOR_BLINK_ON;
    else self->state->usblcd_cursor_blink = 0;
    
    self->control(self);    
}

void picolcd_set_switch(usblcd_operations *self, unsigned int status)
{
    if (status) self->state->usblcd_switch = _USBLCD_SWITCH_ON;
    else self->state->usblcd_switch = 0;
    
    self->control(self);    
}

void picolcd_control(usblcd_operations *self)
{

    char packet[3];
    snprintf(packet, 2 + 1 , "%c%c", OUT_REPORT_LCD_CONTROL, self->state->usblcd_switch | self->state->usblcd_cursor | self->state->usblcd_cursor_blink);
    picolcd_send(self, packet, 2);
}

void picolcd_clear(usblcd_operations *self)
{
    char packet[2];
    snprintf (packet, 1 + 1 , "%c", OUT_REPORT_LCD_CLEAR);
    picolcd_send(self, packet, 2);
}

void picolcd_setchar(usblcd_operations *self, unsigned int row, unsigned int column, char character)
{
    char packet[6];
    
    if (row > self->rows) row = self->rows;
    if (column > self->cols) column = self->cols;
    
    /* message + 4 control bytes row, column,length, character*/
    snprintf (packet, 5 + 1 , "%c%c%c%c%c", OUT_REPORT_LCD_TEXT,  row, column, 1, character);
    picolcd_send(self, packet, 5);
}

void picolcd_settext(usblcd_operations *self, unsigned int row, unsigned int column, char *text)
{
    char packet[64];
    unsigned int len;
    
    len = strlen(text);
    
    if (len > 20) len = 20;
    if (row > self->rows) row = self->rows;
    if (column > self->cols) column = self->cols;
    
    /* 3 control bytes row, column, text length*/
    snprintf(packet, len + 4  + 1 , "%c%c%c%c%s", OUT_REPORT_LCD_TEXT,  row, column, len, text);
    picolcd_send(self, packet, len + 4);
}

void picolcd_getversion(usblcd_operations *self) 
{
    char packet[2];    
    snprintf (packet, 1 + 1 , "%c", HID_REPORT_GET_VERSION);
    picolcd_send(self, packet, 1);
}

void picolcd_enter_flasher_mode(usblcd_operations *self)
{
    char packet[4];
    snprintf(packet, 3 + 1, "%c%c%c", HID_REPORT_EXIT_KEYBOARD, FLASHER_TIMEOUT & 0xFF , (FLASHER_TIMEOUT >> 8) & 0xFF);
    picolcd_send(self, packet, 3);
}

void picolcd_exit_flasher_mode(usblcd_operations *self)
{
    char packet[4];
    
    snprintf(packet, 3 + 1, "%c%c%c", HID_REPORT_EXIT_FLASHER, FLASHER_TIMEOUT & 0xFF , (FLASHER_TIMEOUT >> 8) & 0xFF);
    picolcd_send(self, packet, 3);
}

void picolcd_setfont(usblcd_operations *self, char *filename)
{
    char packet[11];
    usblcd_font_data *buffer, *font;
    buffer = get_font_data(filename, self->max_custom_chars, self->custom_char_width, self->custom_char_height);
    
    if (buffer == NULL) 
	return;
    font = buffer;
    
    while (font->id <= self->max_custom_chars && font->id > 0 ) 
    {
	snprintf(packet, 10 + 1, "%c%c%c%c%c%c%c%c%c%c", OUT_REPORT_LCD_FONT, font->id - 1, 
		bin2int(font->line[0]),
		bin2int(font->line[1]), 
		bin2int(font->line[2]), 
		bin2int(font->line[3]), 
		bin2int(font->line[4]), 
		bin2int(font->line[5]),
		bin2int(font->line[6]), 
		bin2int(font->line[7]));
	
	font++;
	#ifdef DEBUG
	print_buffer(packet, 10);
	#endif
	picolcd_send(self, packet, 10);
    }    	
}

void picolcd_setfont_memory(usblcd_operations *self, int fontlines[], int nrchars)
{
    char packet[11];
    usblcd_font_data *font;
    int i;
    
    for (i = 0; i < nrchars; i++) 
    {
	snprintf(packet, 11 + 1, "%c%c%c%c%c%c%c%c%c%c", OUT_REPORT_LCD_FONT, i, 
		fontlines[i * 8],
		fontlines[i * 8 + 1],
		fontlines[i * 8 + 2],
		fontlines[i * 8 + 3],
		fontlines[i * 8 + 4],
		fontlines[i * 8 + 5],
		fontlines[i * 8 + 6],
		fontlines[i * 8 + 7]);
	font++;
	#ifdef DEBUG
	print_buffer(params->packet, params->packetlen);
	#endif
	picolcd_send(self, packet, 10);
    }   
}


void picolcd_setsplash(usblcd_operations *self, char *filename)
{
    hid_params *params;
    usblcd_splash_data *buffer, *splash;
    char * splashes_text;
    int i;
    int index;
    int offset;
    int splashes_size;
    int splash_size;
    unsigned long ftime = 0;
    unsigned long faddr = 0;
    
    splash_size = (self->cols + 1 + 1);
    splashes_size = splash_size * (self->rows + 1) * self->max_splashes + self->max_splashes * 4; /* 4 control bytes */

    buffer = get_splash_data(filename, self->max_splashes, self->rows, self->cols);
    if (buffer == NULL)
	return;
    
    splashes_text = (char *) malloc (splashes_size);
    if (splashes_text == NULL)
	return;
	
    memset(splashes_text, 0x00, splashes_size);
    
    params =(hid_params *) malloc (sizeof(hid_params));
    params->endpoint = USB_ENDPOINT_OUT + 1;
    params->timeout = HID_TIMEOUT;
    params->packetlen = self->max_data_len;
    params->packet = (char *) malloc ( params->packetlen + 1);
    
    splash = buffer;
    index = 0;
    offset = 0;
    while (index < self->max_splashes) {
    
	if ((splash->min <= -1) || (splash->sec <= -1))
	    ftime = 0xFFFF;
	else 
	    ftime = splash->min * 60 + splash->sec;
	    
	    
	for (i = 0; i < self->rows + 1; i++) {
	    if (i == 0) {
		snprintf(splashes_text + offset, 5 + 1, "%c%c%c%c%c", (int) ftime & 0xFF, (int) (ftime >> 8) & 0xFF, splash->jump, splash->repeat, splash->leds);
		//snprintf(splashes_text + offset, 3 + 1, "%c%c%c", ftime & 0xFF, (ftime >> 8) & 0xFF, splash->leds);
		offset += 5;
		//fprintf (stderr, "\nHeader: %02x%02x%02x%02x%02x\n",ftime & 0xFF, (ftime >> 8) & 0xFF, splash->jump, splash->repeat, splash->leds);
	    }
	    
	    memcpy(splashes_text + offset  , splash->line[i], splash_size);
	    offset = offset +  splash_size;
	}
	
	splash++;
	index++;
    }
    
    offset = 0;
    
    while (offset < splashes_size) {
	memset(params->packet,' ', params->packetlen);
	snprintf(params->packet, 4 + 1, "%c%c%c%c", OUT_REPORT_INT_EE_WRITE, (int)(faddr & 0xFF), (int)((faddr >> 8) & 0xFF), 0x14);
	
	if ((offset + self->max_data_len - 4) > splashes_size) 
	    memcpy(params->packet + 4 , splashes_text + offset, 10);
	else 
	    memcpy(params->packet + 4 , splashes_text + offset, self->max_data_len - 4);
	offset += self->max_data_len - 4;
	faddr += self->max_data_len - 4;
	
	print_buffer(params->packet, params->packetlen);
	index++;
	self->hid->interrupt_write(self->hid->hiddev->handle, params);
	self->read_events(self);
    }    	

    free(params->packet);
    free(params);
    free(buffer);
    free(splashes_text);
}

void picolcd_flash(usblcd_operations *self)
{
    return;
}


/* todo: get keystate from circular buffer */
void picolcd_keystate(usblcd_operations *self)
{

}

/* todo: get irdata from circular buffer */
void picolcd_irdata(usblcd_operations *self)
{

}
/* todo: get powerstate from circular buffer */
void picolcd_powerstate(usblcd_operations *self)
{

}

usblcd_event * picolcd_read_events(usblcd_operations *self)
{
    int ret = -1;
    unsigned char read_packet[self->max_data_len];
    usblcd_event *event;

    //hid_set_idle(self->hid->hiddev->handle, 0, 0);
    
    if ((event = (usblcd_event *) malloc(sizeof(usblcd_event))) == NULL) return NULL;
    if ((event->data = (unsigned char *) malloc(sizeof(unsigned char) * self->max_data_len)) == NULL) 
		    return NULL;
     
    ret = usb_interrupt_read(self->hid->hiddev->handle, USB_ENDPOINT_IN + 1, (char *)read_packet, self->max_data_len, 10000);
    
    if (ret > 0) {
        switch (read_packet[0]) {

	    case IN_REPORT_KEY_STATE:
	    {	
		event->type = 0;
		event->length = 2;
		memcpy(event->data, &read_packet[1], event->length);
#ifdef DEBUG
		print_buffer(event->data, event->length);
#endif	
		return event;
		
	    } break;

    	    case IN_REPORT_IR_DATA:
	    {

		event->type = 1;
		event->length = read_packet[1];
		/* IR data packet also has a IR data length field */
		memcpy(event->data, &read_packet[2], event->length);
#ifdef DEBUG
		print_buffer(event->data, event->length);
#endif	
		return event;

	    } break;

	    case IN_REPORT_INT_EE_DATA:
	    {	
#ifdef DEBUG
		fprintf(stderr,"IN_REPORT_INT_EE_DATA: ");
		print_buffer(read_packet, self->max_data_len);
#endif
	    } break;
		
	    case RESULT_PARAMETER_MISSING:
	    case RESULT_DATA_MISSING:
	    case RESULT_BLOCK_READ_ONLY:
	    case RESULT_BLOCK_TOO_BIG:
	    case RESULT_SECTION_OVERFLOW:
	    case HID_REPORT_GET_VERSION:
	    case HID_REPORT_ERASE_MEMORY:
	    case HID_REPORT_READ_MEMORY:
	    case HID_REPORT_WRITE_MEMORY:
	    case IN_REPORT_EXT_EE_DATA:
	    case OUT_REPORT_EXT_EE_READ:
	    case OUT_REPORT_EXT_EE_WRITE:
	    case OUT_REPORT_INT_EE_READ:
	    case OUT_REPORT_INT_EE_WRITE:
	    case HID_REPORT_EXIT_FLASHER:
	    case HID_REPORT_EXIT_KEYBOARD:
	    default:
		break;
	}
    }
    
    return NULL;
}


void picolcd_close(usblcd_operations *self)
{
    if (self->hid->hiddev->handle) 
	self->hid->close(self->hid->hiddev->handle);
    if (self->hid)
	free(self->hid);
}

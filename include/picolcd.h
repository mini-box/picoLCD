#ifndef __USBLCD_H__
#define __USBLCD_H__

#include <usb.h>
#include "driver.h"

#define VERSION				"0.1.10"

typedef enum _picolcd_drivers_s picolcd_drivers;
enum _picolcd_drivers_s {
    AUTODETECT,
    PICOLCD20x2,
    PICOLCD20x4,
    PICOLCD256x64
};

/* public defs */
static unsigned int debug_level;

typedef int hid_return;

typedef struct _hid_device_s hid_device;
struct _hid_device_s {
    unsigned int id;
    unsigned int vendorid;
    unsigned int productid;
    struct usb_device *device;
    struct usb_dev_handle *handle;
};

typedef struct _hid_params_s hid_params;
struct _hid_params_s {
    unsigned int endpoint;
    unsigned int packetlen;
    unsigned int timeout;
    void *packet;
};

typedef struct _hid_operations_s hid_operations;
struct _hid_operations_s {
    hid_device *hiddev;
    hid_return (*debug)(int level);
    hid_return (*init)(hid_device **hiddev);
    hid_return (*interrupt_read)(void *hiddev, hid_params *params);
    hid_return (*interrupt_write)(void *hiddev, hid_params *params);
    hid_return (*controlled_read)(void);  /* not implemented */
    hid_return (*controlled_write)(void); /* not implemented */
    hid_return (*close)(void *hiddev);
};

typedef struct _usblcd_event_s usblcd_event;
struct _usblcd_event_s {
    /* 0 keypad data, 1 infrared data */
    unsigned int type;
    int length;
    unsigned char *data;
};

typedef struct _usblcd_state_s usblcd_state;
struct _usblcd_state_s {
    unsigned int usblcd_switch;
    unsigned int usblcd_cursor;
    unsigned int usblcd_cursor_blink;
};

typedef struct _usblcd_splash_data_s usblcd_splash_data;
struct _usblcd_splash_data_s {
    int min;
    int sec;
    int jump;
    int repeat;
    unsigned int leds;
    char *line[_USBLCD_MAX_ROWS + 1];
};

typedef struct _usblcd_font_data_s usblcd_font_data;
struct _usblcd_font_data_s {
    int id;
    char *line[_USBLCD_FONT_MAX_ROWS];
};

typedef struct _usblcd_operations_s usblcd_operations;
struct _usblcd_operations_s {
/* hardware features */
    unsigned int vendorid;
    unsigned int productid;
    unsigned int cols;
    unsigned int rows;
    unsigned int max_data_len;
    unsigned int max_leds;
    unsigned int max_splashes;
    unsigned int max_custom_chars;
    unsigned int custom_char_height;
    unsigned int custom_char_width;
/* lcd and cursor status */
    usblcd_state *state;
/* leds state */
    unsigned int leds;
/* misc functions */
    void (*match)(void);
    void (*init)(usblcd_operations *self);
    void (*debug)(int level);
    
/* input functions */    
    void (*setled)(usblcd_operations *self, unsigned int led, unsigned int status);
    void (*backlight)(usblcd_operations *self, unsigned int status);
    void (*contrast)(usblcd_operations *self, unsigned int level);
    void (*set_cursor)(usblcd_operations *self, unsigned int status);
    void (*set_cursor_blink)(usblcd_operations *self, unsigned int status);
    void (*set_switch)(usblcd_operations *self, unsigned int status);
    usblcd_event * (*read_events)(usblcd_operations *self);

    void (*enter_flasher_mode)(usblcd_operations *self);
    void (*exit_flasher_mode)(usblcd_operations *self);
    void (*clear)(usblcd_operations *self);
    void (*setchar)(usblcd_operations *self, unsigned int row, unsigned int column, char character);
    void (*settext)(usblcd_operations *self, unsigned int row, unsigned int column, char *text);
    void (*setfont)(usblcd_operations *self, char *filename);
    void (*setfontmemory)(usblcd_operations *self, int fontlines[], int nrchars);
    void (*setsplash)(usblcd_operations *self, char *filename);
    void (*flash)(usblcd_operations *self);
    void (*getversion)(usblcd_operations *self);
    /* output functions */
    void (*powerstate)(usblcd_operations *self);
    void (*keystate)(usblcd_operations *self);
    void (*irdata)(usblcd_operations *self);

    void (*close)(usblcd_operations *self);
    
    void (*send)(usblcd_operations *self, char *data, int size);
    /* should not be called directly */
    void (*control)(usblcd_operations *self);
    
/* pointer to deeper level hid functions */
    hid_operations *hid;
    
};

/* private class */
/* hid operations constructor */
//static hid_operations *new_hid_operations(void);
hid_operations *new_hid_operations(unsigned int vendorid, unsigned int productid);
/* public classes */
/* usbdriver operations constructor */
usblcd_operations *new_driver(picolcd_drivers driver);

/* functions exported by different drivers */
usblcd_operations * driver_20x2_export(void);
usblcd_operations * driver_20x4_export(void);
usblcd_operations * driver_256x64_export(void);

#endif  //__USBLCD_H__

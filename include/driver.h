#ifndef __DRIVER_H__
#define __DRIVER_H__

/* USB LCD general parameters */

#define _USBLCD_MAX_ROWS		1
#define _USBLCD_MAX_COLS		19
#define _USBLCD_MAX_DATA_LEN		24

#define _USBLCD_SWITCH_ON		4
#define _USBLCD_CURSOR_ON		2
#define _USBLCD_CURSOR_BLINK_ON		1

#define _USBLCD_MAX_LEDS		7

/* IR related define */
#define PULSE_BIT  0x01000000 
#define PULSE_MASK 0x00FFFFFF 

/* USB LCD bootup splash related parameters */
#define _USBLCD_MAX_SPLASHES		5
#define SPLASH_HEADER			":splash"

/* USB LCD font related parameters */
#define FONT_HEADER			":font"
#define _USBLCD_MAX_FONTS		8
#define _USBLCD_FONT_MAX_COLS		5
#define _USBLCD_FONT_MAX_ROWS		8

#define HID_TIMEOUT			1000
#define FLASHER_TIMEOUT			1000

#define OUT_REPORT_LED_STATE		0x81
#define OUT_REPORT_LCD_BACKLIGHT	0x91
#define OUT_REPORT_LCD_CONTRAST		0x92
#define OUT_REPORT_LCD_CONTROL		0x93
#define OUT_REPORT_LCD_CLEAR		0x94
#define OUT_REPORT_LCD_TEXT		0x98
#define OUT_REPORT_LCD_FONT		0x9C

#define HID_REPORT_GET_VERSION          0xF1 

#define IN_REPORT_POWER_STATE		0x01
#define IN_REPORT_KEY_STATE		0x11
#define IN_REPORT_IR_DATA		0x21

#define RESULT_OK			0x00
#define RESULT_PARAMETER_MISSING	0x01            // Request does not contain all necessary parameters (request is too short, no length or no address either).
#define RESULT_DATA_MISSING		0x02		// Request attempts to write more data than was actually sent.
#define RESULT_BLOCK_READ_ONLY		0x03		// Request attempts to erase or write a block within the flasher code section.
#define RESULT_BLOCK_NOT_ERASABLE	0x04            // Request attempts to erase a block outside of "erasable" memory space.
#define RESULT_BLOCK_TOO_BIG 		0x05          	// Request attempts to access more data at once than the HID buffer can handle.
#define RESULT_SECTION_OVERFLOW		0x06		// Request (address & length) attempts to cross over a section boundary (section boundaries are set at 1Mbyte boundaries -> 0x100000, 0x200000... etc.).

#define HID_REPORT_GET_VERSION		0xF1		// No parameters
#define HID_REPORT_ERASE_MEMORY		0xF2		// AddressLo : AddressHi : AddressUp (anywhere inside the 64 byte-block to be erased)
#define HID_REPORT_READ_MEMORY		0xF3		// AddressLo : AddressHi : AddressUp : Data Length (1...32)
#define HID_REPORT_WRITE_MEMORY		0xF4		// AddressLo : AddressHi : AddressUp : Data Length (1...32) : Data....
#define HID_REPORT_EXIT_FLASHER		0xFF		// No parameters
#define HID_REPORT_EXIT_KEYBOARD	0xEF		// No parameters
#define HID_SET_SNOOZE_TIME		0xF8		// 0-65535 (1234h > 34 12);


#define IN_REPORT_EXT_EE_DATA		0x31
#define OUT_REPORT_EXT_EE_READ		0xA1
#define OUT_REPORT_EXT_EE_WRITE		0xA2

#define IN_REPORT_INT_EE_DATA		0x32
#define OUT_REPORT_INT_EE_READ		0xA3
#define OUT_REPORT_INT_EE_WRITE		0xA4

#define TYPE_CODE_MEMORY		0x00
#define TYPE_EPROM_EXTERNAL		0x01
#define TYPE_EPROM_INTERNAL		0x02

#endif  //__DRIVER_H__

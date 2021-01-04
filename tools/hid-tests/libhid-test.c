#include <hid.h>
#include <stdio.h>
#include <string.h>


#include "picolcd.h"
#include "driver.h"


bool match_serial_number(struct usb_dev_handle* usbdev, void* custom, unsigned int len)
{
  bool ret;
  char* buffer = (char*)malloc(len);
  usb_get_string_simple(usbdev, usb_device(usbdev)->descriptor.iSerialNumber,
      buffer, len);
  ret = strncmp(buffer, (char*)custom, len) == 0;
  free(buffer);
  return ret;
}

int main(void)
{
  HIDInterface* hid;
  hid_return ret;

  HIDInterfaceMatcher matcher = { _USBLCD_IDVENDOR, _USBLCD_IDPRODUCT, NULL, NULL, 0 };
  
    

  
  /* see include/debug.h for possible values */
  hid_set_debug(HID_DEBUG_ALL);
  hid_set_debug_stream(stderr);
  /* passed directly to libusb */
  hid_set_usb_debug(0);
  
  ret = hid_init();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_init failed with return code %d\n", ret);
    return 1;
  }

  hid = hid_new_HIDInterface();
  if (hid == 0) {
    fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
    return 1;
  }

  ret = hid_force_open(hid, 0, &matcher, 3);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_force_open failed with return code %d\n", ret);
    return 1;
  }

  ret = hid_write_identification(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_write_identification failed with return code %d\n", ret);
    return 1;
  }

  ret = hid_dump_tree(stdout, hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_dump_tree failed with return code %d\n", ret);
    return 1;
  }


#undef CONTROLED_WRITE
#ifdef CONTROLED_WRITE

    unsigned char const PATHLEN = 2;
    int const PATH_IN[]  = { 0xff000001, 0xff000001 };
    int const PATH_OUT[] = { 0xff000001, 0xff000001 };
    
    unsigned char const SEND_PACKET_LEN = 1;
    char const PACKET[] = { 1};
    

    ret = hid_set_output_report(hid, PATH_IN, PATHLEN, PACKET, SEND_PACKET_LEN);
    if (ret != HID_RET_SUCCESS) {
	fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    }
    
#endif
    /* FUCKING NEW SETUP PACKETS NOBODY TOLD ME ABOUT THEM */
    char const SETUP_PACKET_1[] = { 0x14, 0x03, 0x41, 0x00, 0x43, 0x00, 0x4d, 0x00 };
    char const SETUP_PACKET_2[] = { 0x20, 0x03, 0x41, 0x00, 0x43, 0x00, 0x4d, 0x00 };

    int const SETUP_PACKET_LEN = 8;
    int const WRITE_PACKET_LEN = 2;
    char write_packet[] = { 0x91, 1};
    
    ret = hid_interrupt_write(hid, USB_ENDPOINT_OUT + 1 , SETUP_PACKET_1, SETUP_PACKET_LEN, 10);
    if (ret != HID_RET_SUCCESS) 
	fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);

    ret = hid_interrupt_write(hid, USB_ENDPOINT_OUT + 1 , SETUP_PACKET_2, SETUP_PACKET_LEN, 10);
    if (ret != HID_RET_SUCCESS) 
	fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);

    ret = hid_interrupt_write(hid, USB_ENDPOINT_OUT + 1 , write_packet, WRITE_PACKET_LEN, 10);
    if (ret != HID_RET_SUCCESS) 
	fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);
/*
    write_packet[0] =  0x11;
    ret = hid_interrupt_write(hid, USB_ENDPOINT_OUT + 1 , write_packet, 1, 10);
    if (ret != HID_RET_SUCCESS) 
	fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);

    ret= hid_set_idle(hid, 1,0);
*/
/*
  int const READ_PACKET_LEN = 24;
  char read_packet[24];
  
  int i=0;
  
    ret = hid_interrupt_read(hid, 0x81, read_packet, 1, 10);
    if (ret != HID_RET_SUCCESS) 
	fprintf(stderr, "hid_interrupt_read failed with return code %d\n", ret);
    printf("Read: %x ", read_packet);


  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_close failed with return code %d\n", ret);
    return 1;
  }
*/

    int i;
    unsigned char const PATHLEN = 2;
    int const PATH_OUT[] =  { 0xff000001, 0xff000017 };
    
    unsigned char const PACKET_LEN = 23;
    char PACKET[23];
    memset(PACKET,0,23);

    ret = hid_get_input_report(hid, PATH_OUT, PATHLEN, PACKET, PACKET_LEN);
    if (ret != HID_RET_SUCCESS) {
	fprintf(stderr, "hid_set_output_report failed with return code %d\n", ret);
    } else 
	for (i=0;i<23;i++)
	    printf(" %x ",PACKET[i]);
    

  ret = hid_close(hid);
  hid_delete_HIDInterface(&hid);

  ret = hid_cleanup();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_cleanup failed with return code %d\n", ret);
    return 1;
  }
  
  return 0;
}

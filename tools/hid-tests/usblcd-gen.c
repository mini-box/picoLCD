/* This file is generated with usbsnoop2libusb.pl from a usbsnoop log file. */
/* Latest version of the script should be in http://iki.fi/lindi/usb/usbsnoop2libusb.pl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <usb.h>
#if 1
 #include <linux/usbdevice_fs.h>
 #define LIBUSB_AUGMENT
 #include "libusb_augment.h"
#endif

struct usb_dev_handle *devh;

void release_usb_device(int dummy) {
    int ret;
    ret = usb_release_interface(devh, 0);
    if (!ret)
	printf("failed to release interface: %d\n", ret);
    usb_close(devh);
    if (!ret)
	printf("failed to close interface: %d\n", ret);
    exit(1);
}

void list_devices() {
    struct usb_bus *bus;
    for (bus = usb_get_busses(); bus; bus = bus->next) {
	struct usb_device *dev;
	
	for (dev = bus->devices; dev; dev = dev->next)
	    printf("0x%04x 0x%04x\n",
		   dev->descriptor.idVendor,
		   dev->descriptor.idProduct);
    }
}    

struct usb_device *find_device(int vendor, int product) {
    struct usb_bus *bus;
    
    for (bus = usb_get_busses(); bus; bus = bus->next) {
	struct usb_device *dev;
	
	for (dev = bus->devices; dev; dev = dev->next) {
	    if (dev->descriptor.idVendor == vendor
		&& dev->descriptor.idProduct == product)
		return dev;
	}
    }
    return NULL;
}

void print_bytes(char *bytes, int len) {
    int i;
    if (len > 0) {
	for (i=0; i<len; i++) {
	    printf("%02x ", (int)((unsigned char)bytes[i]));
	}
	printf("\"");
        for (i=0; i<len; i++) {
	    printf("%c", isprint(bytes[i]) ? bytes[i] : '.');
        }
        printf("\"");
    }
}


int main(int argc, char **argv) {
    int ret, vendor, product;
    struct usb_device *dev;
    char buf[65535];
#if 0
    usb_urb *isourb;
    struct timeval isotv;
    char isobuf[32768];
#endif

    usb_init();
    usb_set_debug(255);
    usb_find_busses();
    usb_find_devices();

    if (argc!=3) {
	printf("usage: %s vendorID productID\n", argv[0]);
	printf("ID numbers of currently attached devices:\n");
	list_devices();
	exit(1);
    }
    vendor = strtol(argv[1], NULL, 16);
    product = strtol(argv[2], NULL, 16);
    if (vendor <= 1 || product <= 1) {
	printf("invalid vendor or product id\n");
	exit(1);
    }
    dev = find_device(vendor, product);
    assert(dev);

    devh = usb_open(dev);
    assert(devh);
    
    signal(SIGTERM, release_usb_device);

    ret = usb_get_driver_np(devh, 0, buf, sizeof(buf));
    printf("usb_get_driver_np returned %d\n", ret);
    if (ret == 0) {
	printf("interface 0 already claimed by driver \"%s\", attempting to detach it\n", buf);
	ret = usb_detach_kernel_driver_np(devh, 0);
	printf("usb_detach_kernel_driver_np returned %d\n", ret);
    }
    ret = usb_claim_interface(devh, 0);
    if (ret != 0) {
	printf("claim failed with error %d\n", ret);
		exit(1);
    }
    
    ret = usb_set_altinterface(devh, 0);
    assert(ret >= 0);

ret = usb_get_descriptor(devh, 0x0000001, 0x0000000, buf, 0x0000012);
printf("1 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(5*1000);
ret = usb_get_descriptor(devh, 0x0000002, 0x0000000, buf, 0x0000009);
printf("2 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(5*1000);
ret = usb_get_descriptor(devh, 0x0000002, 0x0000000, buf, 0x0000029);
printf("3 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(9*1000);
ret = usb_release_interface(devh, 0);
if (ret != 0) printf("failed to release interface before set_configuration: %d\n", ret);
ret = usb_set_configuration(devh, 0x0000001);
printf("4 set configuration returned %d\n", ret);
ret = usb_claim_interface(devh, 0);
if (ret != 0) printf("claim after set_configuration failed with error %d\n", ret);
ret = usb_set_altinterface(devh, 0);
printf("4 set alternate setting returned %d\n", ret);
usleep(38*1000);
ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 0x000000a, 0x0000000, 0x0000000, buf, 0x0000000, 1000);
printf("5 control msg returned %d", ret);
printf("\n");
usleep(2*1000);
ret = usb_get_descriptor(devh, 0x0000022, 0x0000000, buf, 0x00000fe);
printf("6 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(32*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("7 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("8 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(2500*1000);
ret = usb_get_descriptor(devh, 0x0000003, 0x0000001, buf, 0x0000202);
printf("9 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(7*1000);
ret = usb_get_descriptor(devh, 0x0000003, 0x0000002, buf, 0x0000202);
printf("10 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(9*1000);
ret = usb_get_descriptor(devh, 0x0000003, 0x0000001, buf, 0x0000202);
printf("11 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(7*1000);
ret = usb_get_descriptor(devh, 0x0000003, 0x0000002, buf, 0x0000202);
printf("12 get descriptor returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(10*1000);
memcpy(buf, "\x81\x00", 0x0000002);
ret = usb_interrupt_write(devh, 0x00000001, buf, 0x0000002, 1000);
printf("13 interrupt write returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(2*1000);
memcpy(buf, "\x91\x01", 0x0000002);
ret = usb_interrupt_write(devh, 0x00000001, buf, 0x0000002, 1000);
printf("14 interrupt write returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(8*1000);
memcpy(buf, "\x92\x00", 0x0000002);
ret = usb_interrupt_write(devh, 0x00000001, buf, 0x0000002, 1000);
printf("15 interrupt write returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(8*1000);
memcpy(buf, "\x98\x00\x00\x14\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 0x0000018);
ret = usb_interrupt_write(devh, 0x00000001, buf, 0x0000018, 1000);
printf("16 interrupt write returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(8*1000);
memcpy(buf, "\x98\x01\x00\x14\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 0x0000018);
ret = usb_interrupt_write(devh, 0x00000001, buf, 0x0000018, 1000);
printf("17 interrupt write returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(2500*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("18 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(153*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("19 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(1127*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("20 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(153*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("21 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(2500*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("22 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(102*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("23 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(2202*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("24 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
usleep(205*1000);
ret = usb_interrupt_read(devh, 0x00000081, buf, 0x0000018, 1000);
printf("25 interrupt read returned %d, bytes: ", ret);
print_bytes(buf, ret);
printf("\n");
	ret = usb_release_interface(devh, 0);
	assert(ret == 0);
	ret = usb_close(devh);
	assert(ret == 0);
	return 0;
}

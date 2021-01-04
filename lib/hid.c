/* Copyright 2006 iTuner Corporation */
/* npavel@ituner.com */

#include <stdio.h>
#include <signal.h>
#include <usb.h>

#include "driver.h"
#include "picolcd.h"
#include "debug.h"


static void release_usb_device(int dummy) 
{
    int ret = -1;
//    ret = usb_release_interface(devh, 0);
    if (!ret)
	printf("failed to release interface: %d\n", ret);
//    usb_close(devh);
    if (!ret)
	printf("failed to close interface: %d\n", ret);

    return;
}


static struct usb_device *_hid_find_device(int vendor, int product) 
{
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

static char * _hid_get_product(struct usb_device *dev, struct usb_dev_handle *devh)
{
    int len;
    int BUFLEN = 256;
    char *buffer;
    
    buffer = (char *) malloc (BUFLEN);
    
    if (buffer == NULL) 
	return NULL;
        
    if (dev->descriptor.iProduct) 
    {
	len = usb_get_string_simple(devh, dev->descriptor.iProduct, buffer, BUFLEN);

	if (len > 0)
	    return buffer;
    }
	free(buffer);
	return NULL;
}

static char * _hid_get_manufacturer(struct usb_device *dev, struct usb_dev_handle *devh)
{
    int len;
    int BUFLEN = 256;
    char *buffer;
    
    buffer = (char *) malloc (BUFLEN);
    
    if (buffer == NULL) 
	return NULL;
        
    if (dev->descriptor.iManufacturer) 
    {
	len = usb_get_string_simple(devh, dev->descriptor.iManufacturer, buffer, BUFLEN);

	if (len > 0)
	    return buffer;
    }
    
    free(buffer);
    return NULL;
}

static char * _hid_get_serial(struct usb_device *dev, struct usb_dev_handle *devh)
{
    int len;
    int BUFLEN = 256;
    char *buffer;
    
    buffer = (char *) malloc (BUFLEN);
    
    if (buffer == NULL) 
	return NULL;
        
    if (dev->descriptor.iSerialNumber) 
    {
	len = usb_get_string_simple(devh, dev->descriptor.iSerialNumber, buffer, BUFLEN);

	if (len > 0)
	    return buffer;
    }
    
    free(buffer);
    return NULL;
}


hid_return _hid_unimplemented(void)
{
    printf("Function not implemented !\n");
    return -1;
}

hid_return _hid_debug(int level)
{
    usb_set_debug(level);
    debug_level = level;
    return 0;
}


hid_return _hid_init(hid_device **hiddev)
{
    hid_return ret;
    char buf[65535];
    
    usb_init();
    
    usb_find_busses();
    
    usb_find_devices();
    
    (*hiddev)->device = _hid_find_device((*hiddev)->vendorid, (*hiddev)->productid);
    
    if ((*hiddev)->device == NULL) {
	printf("Device [0x%04x: 0x%04x] not detected !\n", (*hiddev)->vendorid, (*hiddev)->productid);
	exit (1);
    }
    
    (*hiddev)->handle = usb_open((*hiddev)->device);
    
    signal(SIGTERM, release_usb_device);

    ret = usb_get_driver_np((*hiddev)->handle, 0, buf, sizeof(buf));

    if (ret == 0) {
	printf("interface 0 already claimed by driver \"%s\", attempting to detach it\n", buf);
	ret = usb_detach_kernel_driver_np((*hiddev)->handle, 0);
	printf("usb_detach_kernel_driver_np returned %d\n", ret);
    }
    ret = usb_claim_interface((*hiddev)->handle, 0);
    if (ret != 0) {
	printf("claim failed with error %d\n", ret);
		exit(1);
    }
    
    ret = usb_set_altinterface((*hiddev)->handle, 0);
    
    MESSAGE("Product: %s, Manufacturer: %s, Firmware Version: %s\n", _hid_get_product((*hiddev)->device, (*hiddev)->handle),
	    _hid_get_manufacturer((*hiddev)->device, (*hiddev)->handle), _hid_get_serial((*hiddev)->device, (*hiddev)->handle));
        
    MESSAGE("Found and initialized hid device\n");
    return 0;
}

hid_return _hid_interrupt_read(void *handle, hid_params *params)
{
    hid_return ret;
    ret = usb_interrupt_read((usb_dev_handle *) handle, (unsigned int const) params->endpoint , \
				 (char* const) params->packet, (unsigned int const) params->packetlen, \
				 (unsigned int const) params->timeout);
    
    if (ret < 0) 
	fprintf(stderr, "hid_interrupt_read failed with return code %d\n", ret);
	
    return ret;
}

hid_return _hid_interrupt_write(void *handle, hid_params *params)
{
    hid_return ret;
    
    if (params == NULL)
    {
	printf("Invalid params, skipping write\n");
	return -1;
    }
    
    ret = usb_interrupt_write((usb_dev_handle *) handle, (unsigned int const) params->endpoint , \
				 (char*) params->packet, (unsigned int const) params->packetlen, \
				 (unsigned int const) params->timeout);
    
    if (ret < 0) 
	fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);
	
    return ret;
}

hid_return _hid_close(void *handle)
{
    hid_return ret;
    ret = usb_close(handle);
    
    if (ret < 0) {
	fprintf(stderr, "hid_close failed with return code %d\n", ret);
	return 1;
    }
    
    return 0;
}


hid_operations *new_hid_operations(unsigned int vendorid, unsigned int productid) 
{

    hid_operations *this;
    this = (hid_operations *) malloc(sizeof(hid_operations));
    this->debug = _hid_debug;
    this->init = _hid_init;
    this->interrupt_read = _hid_interrupt_read;
    this->interrupt_write = _hid_interrupt_write;
    this->controlled_read = _hid_unimplemented;
    this->controlled_write = _hid_unimplemented;
    this->close = _hid_close;
    
    this->hiddev = (hid_device *) malloc(sizeof(hid_device));
    this->hiddev->vendorid = vendorid;
    this->hiddev->productid = productid;

    this->debug(0);
    
    if (this->init(&this->hiddev)) {
	fprintf(stderr, "Can't connect to device.\n");
	this->close(this->hiddev);
	free(this);
	return NULL;
    }
    
    return this;
}

#include <stdio.h>


#include "driver.h"
#include "picolcd.h"

usblcd_operations * new_driver(unsigned int driver) {

    switch(driver) 
    {
	case AUTODETECT:
	{
	    /* not implemented yet */
	    return NULL;
	}
	case PICOLCD20x2: 
	{
	    return (usblcd_operations *) driver_20x2_export();

	} break;
	case PICOLCD20x4: 
	{
	    return (usblcd_operations *) driver_20x4_export();
	
	} break;
	
	case PICOLCD256x64: 
	{
	    return (usblcd_operations *)  driver_256x64_export();
	
	} break;
	
	default:
	{	
	    fprintf(stderr, "Unknown driver specified\n");
	    return NULL;
	}
    };
}

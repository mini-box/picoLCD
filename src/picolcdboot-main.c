/* Copyright 2006 iTuner Corporation */
/* npavel@ituner.com */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "picolcd.h"
#include "picolcd-util.h"



int main (int argc, char **argv)
{
    usblcd_operations *mylcd;

    /* init hid device and usblcd_operations structure */
    mylcd = new_driver(PICOLCD20x2);
    /* set hid debug level */
    mylcd->hid->debug(0);
    /* set usblcd debug level */
    mylcd->debug(0);
    /* init the USB LCD */
    mylcd->init(mylcd);
    /* clear the LCD screen */
    mylcd->clear(mylcd);
    /* clear all leds status */
    mylcd->setled(mylcd, 0, 0);
    /* set default cursor */
    mylcd->set_cursor(mylcd, 0);    
    /* set default non blink cursor */
    mylcd->set_cursor_blink(mylcd, 0);    
    mylcd->backlight(mylcd, 1);
    
    mylcd->setsplash(mylcd,"/etc/usblcd/splash.conf");

    mylcd->setfont(mylcd,"/etc/usblcd/minibox-logo.fnt");
    mylcd->setchar(mylcd, 0, 0, 0); 
    mylcd->setchar(mylcd, 1, 0, 1); 
    
    mylcd->setchar(mylcd, 0, 1, 2); 
    mylcd->setchar(mylcd, 1, 1, 3); 
    
    mylcd->setchar(mylcd, 0, 2, 4); 
    mylcd->setchar(mylcd, 1, 2, 5); 
    
    mylcd->setchar(mylcd, 0, 3, 6); 
    mylcd->setchar(mylcd, 1, 3, 7); 

    mylcd->settext(mylcd, 1, 4, "ini-box.com 2006");
    mylcd->close(mylcd);
    return 0;
}

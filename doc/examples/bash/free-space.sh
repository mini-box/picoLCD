#!/bin/bash
 DRIVE1=/dev/sda1
 DRIVE2=/dev/sdb1
 picolcd clear > /dev/null 2>&1
 picolcd backlight 1 > /dev/null 2>&1
 while [ 1 ]; do 
    e=`df -h -l | grep -m 1 $DRIVE1 | cut -d '/' -f 3 | tail -c 5`
    usblcd text 0 1 "$DRIVE1 used:" text 0 17 $e > /dev/null 2>&1
    e=`df -h -l | grep -m 1 $DRIVE2 | cut -d '/' -f 3 | tail -c 5`
    usblcd text 1 1 "$DRIVE2 used:" text 1 17 $e > /dev/null 2>&1
    sleep 2
 done
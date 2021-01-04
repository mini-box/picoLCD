#!/bin/bash
 picolcd clear > /dev/null 2>&1
 picolcd backlight 1 > /dev/null 2>&1
 while [ 1 ]; do 
    e=`vmstat`
    e=`echo $e | cut -d ' ' -f 37`
    picolcd text 0 1 " Idle CPU:" text 0 13 $e% > /dev/null 2>&1
    sleep 2
 done
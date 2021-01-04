#!/bin/sh

# Script to run lcd4linux since you can't pass more then 1 parameter to InstallJammer shortcut creation function 
# we need a script for each shortcut

if [ $# -lt 1 ]
then
    INSTALL_PATH=$HOME/picolcd-256x64
else
    INSTALL_PATH=$1
fi

echo "Installation path: $INSTALL_PATH"

if [ ! -e $INSTALL_PATH/lcd4linux ] 
then
    echo -e "Cannot find lcd4linux executable file ! \nCheck if you installed the program correctly"
    exit 1
fi

WRAPPER="$INSTALL_PATH/picoLCDGraphic/picolcd-wrapper.sh $INSTALL_PATH lcd4linux.conf"
LD_LIBRARY_PATH=$INSTALL_PATH/picoLCDGraphic/Libs/:$LD_LIBRARY_PATH 
PYTHONPATH=$INSTALL_PATH/picoLCDGraphic/Scripts/:$PYTHONPATH

export GKSU_HELPER_DIR="$INSTALL_PATH/picoLCDGraphic/Libs/libgksu"
export LD_LIBRARY_PATH 
export PYTHONPATH

# picoLCD drivers need root access to detach from the kernel HID driver

if [ $USER != "root" ]
then
    echo "Running gksu wrapper"
    $INSTALL_PATH/picoLCDGraphic/gksu --message "picoLCD driver needs to be restarted"  "killall -9 lcd4linux"
    $INSTALL_PATH/picoLCDGraphic/gksu --message "picoLCD needs to detach from the kernel HID driver to function correctly" "$WRAPPER"
else
    killall -9 lcd4linux
    $INSTALL_PATH/lcd4linux -f $INSTALL_PATH/picoLCDGraphic/Configs/lcd4linux.conf -q
fi






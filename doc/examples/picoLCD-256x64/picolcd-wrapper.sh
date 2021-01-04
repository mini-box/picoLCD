#!/bin/sh

# because in Ubuntu gksu doesn't send the environment from gksu to the lcd4linux
# we need to pass thru a wrapper

if [ $# -lt 1 ]
then
    INSTALL_PATH=$HOME/picolcd-256x64
    CONFIGFILE="lcd4linux.conf"

else
    INSTALL_PATH=$1
    CONFIGFILE=$2
fi

LD_LIBRARY_PATH="$INSTALL_PATH/picoLCDGraphic/Libs/:$LD_LIBRARY_PATH"
PYTHONPATH="$INSTALL_PATH/picoLCDGraphic/Scripts/:$PYTHONPATH"
CONFIG="$INSTALL_PATH/picoLCDGraphic/Configs/$CONFIGFILE"

echo "Config: $CONFIG, PYTHONPATH: $PYTHONPATH"

if [ ! -e $INSTALL_PATH/lcd4linux ] 
then
    echo -e "Cannot find lcd4linux executable file ! \nCheck if you installed the program correctly"
    exit 1
fi

export LD_LIBRARY_PATH 
export PYTHONPATH

$INSTALL_PATH/lcd4linux -f $CONFIG -F -q








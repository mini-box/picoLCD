# picoLCD - linux driver and SDK for Mini-Box.com USB LCD

[picoLCD](https://www.mini-box.com/picoLCD) is a family of Intelligent USB LCD displays with true HID USB interface. Current end existent products range from simple 2x20 LCDs, 4x20 as well as graphic models (256x64). The picoLCD has strong application support (SDK included) for Windows XP and Linux as well as native Vista SideShow auxiliary display driver support and SDK.

![256x64](https://www.mini-box.com/core/media/media.nl?id=18687&c=ACCT127230&h=3cde0d8a695a00886f7b)
![20x2](https://www.mini-box.com/core/media/media.nl?id=18681&c=ACCT127230&h=13d54ca450368cba4c0e)

## Features

- True full-speed USB HID device
- USB full-speed 2.0 support
- 10 custom splash screens, custom timing / looping
- IR receiver connector
- LIRC support (20x2, 20x4)
- Keypad / LED connector
- USART and I2C connectors
- Supports 8 GPO
- Custom fonts and splash screens


## Building
        ./autogen.sh
        make install

## Running on MacOS

SDK supports all models and can be built on MacOS. The 256x64 is not yet fully supported. It also supports MacOS leopard and snow leopard. For MacOS a codeless kext must be installed before SDK can be used. See the kext/ folder inside archive.

## Examples

See [examples](doc/examples/) folder 

## lcdproc, lcd4linux, kernel patches 
*already included in mainline no longer needed*
See [patches](patches/) folder

## Ubuntu packages (ppa)
*no longer updated*
[Ubuntu PPA](https://launchpad.net/~mini-box.com/+archive/ubuntu/ppa)

## Using the SDK library

See [libusblcd](doc/libusblcd-development-guide.txt) documentation

## Using the picoLCD app
See [picolcd](doc/usblcd-client-guide.txt) documentation

    

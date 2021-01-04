AM_MAKEFLAGS = 
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS =
AM_CFLAGS =  -fPIC -Wall $(USB_CFLAGS)
AM_LDFLAGS = 

if OS_LINUX
AM_CFLAGS +=
AM_LDFLAGS += -lrt
endif

if OS_MACOSX
AM_CFLAGS += -D__MACOS__
AM_LDFLAGS += 
endif


bin_PROGRAMS = picolcd picolcdboot
picolcd_DEPENDENCIES = ../lib/libpicolcd.la
picolcd_LDADD = ../lib/libpicolcd.la $(AM_LDFLAGS)
picolcd_SOURCES = main.c picolcd-util.c

picolcdboot_DEPENDENCIES = ../lib/libpicolcd.la
picolcdboot_LDADD = ../lib/libpicolcd.la $(AM_LDFLAGS)
picolcdboot_SOURCES = picolcdboot-main.c picolcd-util.c

include_HEADERS = ../include/picolcd.h ../include/driver.h ../include/picolcd-util.h ../include/widgets.h 

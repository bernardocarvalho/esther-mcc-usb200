#############################################################################
#	
#	makefile for building:
#
#		test-usb1208LS:      Program to test USB-1208LS module
#		test-usb1024LS:      Program to test USB-1024LS module
#               test-usb-20X         Program to test the USB-201 and USB-204 devices.
###########################################################################

#  Current Version of the driver
#VERSION=1.63

SRCS = Plus.c usb-2020.c
#HEADERS = pmd.h usb-1208LS.h usb-1208FS.h usb-1208HS.h usb-1408FS.h usb-1608FS.h usb-1608HS.h usb-1608G.h usb-1024LS.h usb-tc.h usb-tc-ai.h usb-temp.h usb-52XX.h usb-dio96H.h usb-1616FS.h minilab-1008.h usb-ssr.h usb-500.h usb-3100.h usb-pdiso8.h usb-erb.h usb-4303.h usb-dio24.h usb-2416.h usb-20X.h usb-1608FS-Plus.h usb-2600.h usb-ctr.h usb-1208FS-Plus.h usb-2020.h
OBJS = $(SRCS:.c=.o)   # same list as SRCS with extension changed
CC=gcc
CFLAGS= -g -Wall -fPIC -O 
TARGETS=test-usb20X test200 triggerADC

ID=MCCLIBHID
DIST_NAME=$(ID).$(VERSION).tgz


###### RULES
all: $(TARGETS)

#%.d: %.c
#	set -e; $(CC) -I. -M $(CPPFLAGS) $< \
#	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
#	[ -s $@ ] || rm -f $@
#ifneq ($(MAKECMDGOALS),clean)
#include $(SRCS:.c=.d)
#endif


test-usb20X: test-usb20X.c usb-20X.o libmcchid.a
	$(CC) -g -Wall -I.. -o $@  $@.c -L.. -lmcchid  -lm -L/usr/local/lib -lhid -lusb

test200: test200.c
	$(CC) -g -Wall -I.. -o $@  $@.c -L.. -lmcchid  -lm -L/usr/local/lib -lhid -lusb

triggerADC: triggerADC.c
	$(CC) -g -Wall -I.. -o $@  $@.c -L.. -lmcchid  -lm -L/usr/local/lib -lhid -lusb


data2ascii: data2ascii.c
	$(CC) -g -Wall -I.. -o $@  $@.c -L.. 


clean:
	rm -rf *.d *.o *~ *.a *.so $(TARGETS)


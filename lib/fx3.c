#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <libusb.h>

#include "libfx3load.h"
#include "libfx3load-private.h"

#define LIBUSB_TIMEOUT 100

fx3_t fx3load_open(uint16_t vid, uint16_t pid)
{
	libusb_context *usb;
	libusb_device_handle *usb_handle;

	int ret = libusb_init(&usb);
	if(ret) {
		return NULL;
	}

	usb_handle = libusb_open_device_with_vid_pid(usb, vid, pid);
	if(!usb_handle) {
		libusb_exit(usb);

		return NULL;
	}

	fx3_t fx3 = malloc(sizeof(*fx3));

	fx3->usb_ctx = usb;
	fx3->usb_dev = usb_handle;

	fx3load_error_clear(fx3);

	return fx3;
}

void fx3load_close(fx3_t fx3)
{
	libusb_close(fx3->usb_dev);
	libusb_exit(fx3->usb_ctx);

	free(fx3);
}

static int do_control_transfer(fx3_t fx3, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue,
		uint16_t wIndex, unsigned char *data, uint16_t wLength)
{
	fx3load_error_clear(fx3);

	int ret = libusb_control_transfer(fx3->usb_dev, bmRequestType, bRequest,
			wValue, wIndex,
			data, wLength, LIBUSB_TIMEOUT);

	if(ret < 0) {
		fx3load_error_set(fx3, "libusb_control_transfer returned: %s", libusb_error_name(ret));
		return -1;
	}

	return 0;
}

int fx3load_write(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data)
{
	return do_control_transfer(fx3, 0x40, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			data, len);
}

int fx3load_read(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data)
{
	return do_control_transfer(fx3, 0xC0, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			data, len);
}

int fx3load_exec(fx3_t fx3, uint32_t addr)
{
	return do_control_transfer(fx3, 0x40, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			NULL, 0);
}

const char *fx3load_error(fx3_t fx3)
{
	if(fx3->errbuf[0] == 0)
		return NULL;

	return fx3->errbuf;
}

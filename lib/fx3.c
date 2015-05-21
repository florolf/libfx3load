#include <stdlib.h>
#include <stdint.h>

#include <libusb.h>

#include "libfx3load.h"
#include "libfx3load-private.h"

fx3_t fx3load_open(uint16_t vid, uint16_t pid) {
	libusb_context *usb;
	libusb_device_handle *usb_handle;

	int ret = libusb_init(&usb);
	if(ret)
		return NULL;

	usb_handle = libusb_open_device_with_vid_pid(usb, vid, pid);
	if(!usb_handle) {
		libusb_exit(usb);
		return NULL;
	}

	fx3_t fx3 = malloc(sizeof(*fx3));

	fx3->usb_ctx = usb;
	fx3->usb_dev = usb_handle;

	return fx3;
}

void fx3load_close(fx3_t fx3) {
	libusb_close(fx3->usb_dev);
	libusb_exit(fx3->usb_ctx);
	free(fx3);
}

int fx3load_write(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data) {
	return libusb_control_transfer(fx3->usb_dev, 0x40, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			data, len, 0);
}

int fx3load_read(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data) {
	return libusb_control_transfer(fx3->usb_dev, 0xC0, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			data, len, 0);
}

int fx3load_exec(fx3_t fx3, uint32_t addr) {
	return libusb_control_transfer(fx3->usb_dev, 0x40, 0xA0,
			addr & 0xFFFF,
			(addr >> 16) & 0xFFFF,
			NULL, 0, 0);
}

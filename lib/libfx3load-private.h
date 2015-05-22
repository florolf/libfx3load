#pragma once

#include <libusb.h>
#include <stdio.h>

#define ERRBUF_LEN 128

struct fx3 {
	libusb_context *usb_ctx;
	libusb_device_handle *usb_dev;

	char errbuf[ERRBUF_LEN];
};

#define fx3load_error_set(fx3,...) snprintf((fx3)->errbuf, ERRBUF_LEN, ##__VA_ARGS__)
#define fx3load_error_clear(fx3,...) (fx3)->errbuf[0] = 0

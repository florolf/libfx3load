#pragma once

#include <libusb.h>

struct fx3 {
	libusb_context *usb_ctx;
	libusb_device_handle *usb_dev;
};

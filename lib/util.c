#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "libfx3load.h"

static uint32_t hex2bin(const char *p, size_t n) {
	uint32_t ret = 0;

	while(n--) {
		ret <<= 4;

		int c = *p++ | 0x20;

		if(c >= '0' && c <= '9')
			ret |= c - '0';
		else if(c >= 'a' && c <= 'f')
			ret |= c - 'a' + 10;
	}

	return ret;
}

static int parse_line(const char *line, uint16_t *offset, uint8_t *type, uint8_t *data) {
	const char *p = line;

	if(strlen(line) < 11)
		return -1;

	if(*p != ':')
		return -1;

	p += 1;

	uint8_t len;
	len = hex2bin(p, 2);
	p += 2;

	*offset = hex2bin(p, 4);
	p += 4;

	*type = hex2bin(p, 2);
	p += 2;

	if(len > (strlen(line) - 11) * 2)
		return -1;

	for(uint8_t i = 0; i < len; i++) {
		data[i] = hex2bin(p, 2);
		p += 2;
	}

	return len;
}

int fx3load_from_ihex(fx3_t fx3, const char *path) {
	int ret;
	FILE *f;

	if((f = fopen(path, "r")) == NULL) {
		return -1;
	}

	uint32_t base_address = 0;

	char buf[1024];
	uint8_t data[256];
	while(fgets(buf, 1024, f)) {
		uint8_t type;
		uint16_t offset;

		int len = parse_line(buf, &offset, &type, data);

		if(len < 0)
			continue;

		uint32_t addr = base_address + offset;

		if(type == 0x00) { // data
			ret = fx3load_write(fx3, addr, len, data);

			if(ret < 0)
				return ret;
		} else if(type == 0x01) { // eof
			break;
		} else if(type == 0x04) { // set base address
			base_address =
				(data[0] << 24) |
				(data[1] << 16);
		} else if(type == 0x05) { // execute program
			addr =
				data[0] << 24 |
				data[1] << 16 |
				data[2] <<  8 |
				data[3];

			ret = fx3load_exec(fx3, addr);

			if(ret < 0)
				return ret;
		}
	}

	fclose(f);

	return 0;
}

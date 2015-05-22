#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "libfx3load.h"

int main(int argc, char **argv) {
	if(argc != 4) {
		fprintf(stderr, "usage: %s VID PID file.ihex\n", argv[0]);
		return EXIT_FAILURE;
	}

	uint16_t vid, pid;
	vid = strtol(argv[1], NULL, 16);
	pid = strtol(argv[2], NULL, 16);

	fx3_t fx3 = fx3load_open(vid, pid);
	if(!fx3) {
		fprintf(stderr, "failed to open fx3\n");
		return EXIT_FAILURE;
	}

	int ret;
	ret = fx3load_from_ihex(fx3, argv[3]);
	if(ret < 0) {
		fprintf(stderr, "fx3load_from_ihex returned %d\n", ret);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

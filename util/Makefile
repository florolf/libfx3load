all:
	$(CC) -I../lib/ fx3load.c -Wall -Wextra -o fx3load `pkg-config --libs libusb-1.0` ../lib/libfx3load.a

clean:
	rm -f fx3load

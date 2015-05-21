OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

CFLAGS=-Wall -Wextra -std=c99 `pkg-config --cflags libusb-1.0`

all: libfx3load.a

clean:
	rm -f *.o *.a

libfx3load.a: $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $^

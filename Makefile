all: lib util

lib:
	make -C lib

util:
	make -C util

clean:
	make -C lib clean

.PHONY: lib util

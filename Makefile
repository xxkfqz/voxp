PACKAGE = voxp

CC ?= gcc

INCLUDE = include
SRC = $(wildcard ./src/*.c)

ifeq ($(MAKECMDGOALS),debug)
CFLAGS ?= -O0 -g -Wall
else
CFLAGS ?= -O3
endif
CFLAGS += --std=c11 -c -I $(INCLUDE) -D PACKAGE_NAME=\"$(PACKAGE)\"
LDFLAGS = -ldl -lm -o $(PACKAGE)

all debug:
	$(CC) $(CFLAGS) $(SRC)
	$(CC) ./*.o $(LDFLAGS)

clean:
	rm -fv ./*.o ./*.wav $(PACKAGE)

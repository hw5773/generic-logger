CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

BIN = test
OBJS = test.o generic.o 

CFLAGS += -DDEBUG

.PHONY: all
all: test

test: $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o: 
	$(CC) -c $<

clean:
	rm -f $(BIN) $(OBJS)

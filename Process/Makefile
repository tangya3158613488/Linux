SOURCE=$(wildcard *.c)
TARGETS=$(patsubst %.c,%,$(SOURCE))

CC=gcc
CFLAGS= -g -Wall

all:$(TARGETS)

$(TARGETS):%:%.c
	$(CC) $< $(CFLAGS) -o $@
.PHONY:clean all
clean:
	rm -rf $(TARGET)

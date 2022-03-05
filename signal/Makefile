CFLAGS := -Wall -Werror
CC := gcc
DEBUG := false

ifeq ($(DEBUG), true)

CFLAGS += -g

endif

all: signal_recv signal_send 

signal_recv: signal_recv.c
	$(CC) $(CFLAGS) $< -o $@ 

signal_send: signal_send.c
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf signal_recv signal_send

.PHONY: all clean
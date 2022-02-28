all: client server

client: client.c
	gcc -Wall -Werror $< -o $@

server: server.c
	gcc -Wall -Werror $< -o $@

clean:
	rm -rf client server

.PHONY: all clean
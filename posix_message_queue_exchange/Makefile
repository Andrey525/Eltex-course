all: client server

client: client.c
	gcc -Wall -Werror $< -o $@ -lrt

server: server.c
	gcc -Wall -Werror $< -o $@ -lrt

clean:
	rm -rf client server

.PHONY: all clean
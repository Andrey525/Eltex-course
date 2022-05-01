all: bin bin/serial_server bin/client

bin/serial_server: src/serial_server.c
	gcc -Wall $< -o $@

bin/client: src/client.c
	gcc -Wall $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin

.PHONY: all clean 
all: bin bin/serial_server_udp bin/client_raw_udp

bin/serial_server_udp: src/serial_server_udp.c
	gcc -Wall $< -o $@

bin/client_raw_udp: src/client_raw_udp.c
	gcc -Wall $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin

.PHONY: all clean 
all: bin bin/serial_server_udp_ip bin/client_raw_udp_ip

bin/serial_server_udp_ip: src/serial_server_udp_ip.c
	gcc -Wall $< -o $@

bin/client_raw_udp_ip: src/client_raw_udp_ip.c
	gcc -Wall $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin

.PHONY: all clean 
# example:
# make SERVER=serial_server_tcp CLIENT=client_tcp


all: bin bin/$(SERVER) bin/$(CLIENT)

bin/$(SERVER): src/$(SERVER).c
	gcc -Wall -Werror $< -o $@

bin/$(CLIENT): src/$(CLIENT).c
	gcc -Wall -Werror $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin

.PHONY: all clean 
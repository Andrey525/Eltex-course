all: bin build bin/bebrash

bin/bebrash: build/main.o build/bebrash.o
	gcc $^ -o $@

build/bebrash.o: src/bebrash.c src/bebrash.h
	gcc -Wall -Werror -c src/bebrash.c -o $@

build/main.o: src/main.c src/bebrash.h
	gcc -Wall -Werror -c src/main.c -o $@

build:
	mkdir build

bin: 
	mkdir bin

clean:
	rm -rf bin build

.PHONY: all clean
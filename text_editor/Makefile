all: bin build bin/prog

bin/prog: build/main.o
	gcc build/main.o -o bin/prog -lncurses

build/main.o: src/main.c
	gcc -Wall -Werror -c src/main.c -o build/main.o

bin:
	mkdir bin

build:
	mkdir build

clean: 
	rm -rf bin build

.PHONY: all clean

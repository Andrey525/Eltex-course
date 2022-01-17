all: bin bin/prog1 bin/prog2 bin/prog3

bin/prog1: src/main1.c
	gcc -Wall -Werror src/main1.c -o $@

bin/prog2: src/main2.c
	gcc -Wall -Werror src/main2.c -o $@

bin/prog3: src/main3.c
	gcc -Wall -Werror src/main3.c -o $@

bin:
	mkdir bin

clean:
	rm -rf bin file.txt

.PHONY: all clean

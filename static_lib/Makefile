CC := gcc
CFLAGS := -Wall -Werror -c
LDFLAGS :=
SOURCES := src/sum.c src/subtraction.c src/multiplication.c src/division.c
OBJECTS := build/sum.o build/subtraction.o build/multiplication.o build/division.o
TARGET := bin/calc

.PHONY: all clean install uninstall

all: bin build lib $(TARGET)

$(TARGET): build/main.o lib/libcalc.a
	$(CC) build/main.o -L./lib -lcalc -o $(TARGET)

lib/libcalc.a: $(OBJECTS)
	ar rc lib/libcalc.a $(OBJECTS)

build/main.o: src/main.c src/calculator.h
	$(CC) $(CFLAGS) $< -o $@

build/sum.o: src/sum.c src/calculator.h
	$(CC) $(CFLAGS) $< -o $@

build/subtraction.o: src/subtraction.c src/calculator.h
	$(CC) $(CFLAGS) $< -o $@

build/multiplication.o: src/multiplication.c src/calculator.h
	$(CC) $(CFLAGS) $< -o $@

build/division.o: src/division.c src/calculator.h
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir bin

build:
	mkdir build

lib:
	mkdir lib

clean:
	rm -rf build lib bin

# $@ Имя цели обрабатываемого правила
# $< Имя первой зависимости обрабатываемого правила
# $^ Список всех зависимостей обрабатываемого правила
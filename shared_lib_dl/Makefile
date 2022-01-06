CC := gcc
CFLAGS := -Wall -Werror -c -fPIC
LDFLAGS :=
SOURCES := src/sum.c src/subtraction.c src/multiplication.c src/division.c
OBJECTS := build/sum.o build/subtraction.o build/multiplication.o build/division.o
LIBRARIES := lib/libsum.so lib/libsubtraction.so lib/libmultiplication.so lib/libdivision.so
TARGET := bin/calc

.PHONY: all clean install uninstall

all: bin build lib $(TARGET)

$(TARGET): build/main.o $(LIBRARIES)
	$(CC) build/main.o -o $(TARGET) -ldl

##########################################

lib/libsum.so: build/sum.o
	$(CC) -shared $< -o $@

lib/libsubtraction.so: build/subtraction.o
	$(CC) -shared $< -o $@

lib/libmultiplication.so: build/multiplication.o
	$(CC) -shared $< -o $@

lib/libdivision.so: build/division.o
	$(CC) -shared $< -o $@

##########################################

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
	
###########################################

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
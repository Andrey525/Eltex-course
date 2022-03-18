all: bin build bin/prog

bin/prog: build/Main.o build/Gui.o
	g++ -no-pie $^ -o bin/prog -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lsfml-audio -ltgui

build/Main.o: src/Main.cpp src/Gui.hpp
	g++ -Wall -Werror -c src/Main.cpp -o $@

build/Gui.o: src/Gui.cpp src/Gui.hpp
	g++ -Wall -Werror -c src/Gui.cpp -o $@

bin:
	mkdir bin

build:
	mkdir build

clean: 
	rm -rf bin build

.PHONY: all clean

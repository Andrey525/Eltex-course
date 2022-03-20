all: bin build bin/prog

bin/prog: build/Main.o build/Gui.o build/File.o
	g++ $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lsfml-audio -ltgui

build/Main.o: src/Main.cpp src/Gui.hpp src/File.hpp
	g++ -Wall -Werror -c $< -o $@ -g

build/Gui.o: src/Gui.cpp src/Gui.hpp src/File.hpp
	g++ -Wall -Werror -c $< -o $@ -g

build/File.o: src/File.cpp src/File.hpp
	g++ -Wall -Werror -c $< -o $@ -g

bin:
	mkdir bin

build:
	mkdir build

clean: 
	rm -rf bin build

.PHONY: all clean

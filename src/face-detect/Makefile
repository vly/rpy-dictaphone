all: face-detect

face-detect: main.o
	g++ main.o -o face-detect `pkg-config --cflags --libs opencv`

main.o: main.cpp
	g++ -Wall -c main.cpp `pkg-config --cflags --libs opencv`

clean:
	rm -rf *o face-detect

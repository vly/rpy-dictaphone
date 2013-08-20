EXE=rand-face-detect
CC=g++
CFLAGS=-std=c++0x

all: $(EXE)

$(EXE): main.o
	$(CC) main.o -o $(EXE) $(CFLAGS)

main.o: main.cpp
	$(CC) -Wall -c main.cpp $(CFLAGS)

clean:
	rm -rf *o $(EXE)

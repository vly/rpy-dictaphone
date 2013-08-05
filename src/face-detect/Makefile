EXE=face-detect
CC=g++
CFLAGS=$(shell pkg-config --cflags --libs opencv)

all: $(EXE)

$(EXE): main.o
	$(CC) main.o -o $(EXE) $(CFLAGS)

main.o: main.cpp
	$(CC) -Wall -c main.cpp $(CFLAGS)

clean:
	rm -rf *o $(EXE)

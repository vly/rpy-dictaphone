EXE=pir-monitor
CC=gcc
CFLAGS=-l bcm2835

all: $(EXE)

$(EXE): main.o
	$(CC) main.o -o $(EXE) $(CFLAGS)

main.o: main.c
	$(CC) -Wall -c main.c $(CFLAGS)

clean:
	rm -rf *o $(EXE)
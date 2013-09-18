CC=g++
CFLAGS=-c -Wall -g
//LDFLAGS = $(shell pkg-config --cflags --libs opencv)
LDFLAGS = -lpthread
SOURCES=main_client.cpp message.cpp message_query.cpp comm.cpp client.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=osp_client

all:$(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@ 

clean:
	-rm -f *.o
	-rm -f $(EXECUTABLE)

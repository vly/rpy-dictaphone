CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = $(shell pkg-config --cflags --libs opencv)
SOURCES=main_server.cpp face_detect.cpp message.cpp message_query.cpp comm.cpp server.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=face_detect

all:$(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@ 

clean:
	-rm -f *.o
	-rm -f $(EXECUTABLE)

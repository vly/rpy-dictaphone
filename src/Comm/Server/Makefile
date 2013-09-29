CC=g++
CFLAGS=-c -Wall -g 
LDFLAGS = $(shell pkg-config --cflags --libs opencv) 
LDFLAGS_EX = -lbcm2835 
SOURCES=main_server.cpp face_detect.cpp message.cpp message_query.cpp comm.cpp server.cpp pir_monitor.cpp maestro.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=osp_server

all:$(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDFLAGS_EX) -o $@ 

.cpp.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $(LDFLAGS_EX) $< -o $@ 

clean:
	-rm -f *.o
	-rm -f $(EXECUTABLE)

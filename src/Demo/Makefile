CC=g++
CFLAGS=-c -g -lpthread
LDFLAGS = $(shell pkg-config --cflags --libs opencv)
LDFLAGS_EX = -lbcm2835
SOURCES=main.cpp face_detect.cpp maestro.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=face_detect

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDFLAGS_EX) -o $@

.cpp.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $(LDFLAGS_EX) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(EXECUTABLE)

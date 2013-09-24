CC := g++
CFLAGS := $(shell pkg-config --cflags --libs opencv)
DBGFLAGS := -g -DDEBUG
RM := rm -f
EXE := face-detect

SOURCES := main.cpp maestro.cpp
OBJECTS := $(SOURCES:%.cpp=%.o)

DEPEND := $(OBJECTS:%.o=%.d)
-include $(DEPEND)

all : $(OBJECTS)
	$(CC) -o $(EXE) $(CFLAGS) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean :
	$(RM) $(OBJECTS)

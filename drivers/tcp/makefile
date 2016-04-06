CC=g++
CFLAGS=-c -pthread --std=c++11
LDFLAGS=-lpthread
EXECUTABLE=client.x

SOURCES=tcpipnix.cpp client.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: clean cleanall
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp $(wildcard *.h)
	$(CC) $(CFLAGS) $< -o $@

cleanall: clean
	rm -f $(EXECUTABLE)

clean:
	rm -f *.o *.gch

run:
	./$(EXECUTABLE)

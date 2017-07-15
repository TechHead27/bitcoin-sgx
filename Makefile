CC=g++
CXXFLAGS=-Wall -D_GNU_SOURCE $(shell pkg-config --cflags libbitcoin-client)
LDLIBS=-lcrypto $(shell pkg-config --libs libbitcoin-client)
SOURCE=$(wildcard *.cpp)
OBJ=$(SOURCE:.cpp=.o)
EXECUTABLE=ransom

Debug: CXXFLAGS += -Og -g

Release: CXXFLAGS += -O

.PHONY: Release Debug clean

Release: $(EXECUTABLE)

Debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LOADLIBS) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

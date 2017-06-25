CC=gcc
CFLAGS=-Wall -D_GNU_SOURCE -O -g
LDLIBS=-lcrypto
SOURCE=encrypt.cpp main.cpp
OBJ=$(SOURCE:.cpp=.o)
EXECUTABLE=ransom

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LOADLIBES) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

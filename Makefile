CC=gcc
CFLAGS=-Wall -D_GNU_SOURCE -O -g
LDLIBS=-lcrypto
SOURCE=encrypt.c main.c
OBJ=$(SOURCE:.c=.o)
EXECUTABLE=ransom

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LOADLIBES) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

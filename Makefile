CC=gcc
CFLAGS=-Wall -D_GNU_SOURCE -O -g
LDLIBS=-lcrypto
SOURCE=encrypt.c main.c
OBJ=$(SOURCE:.c=.o)
EXECUTABLE=ransom.out

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LOADLIBS) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

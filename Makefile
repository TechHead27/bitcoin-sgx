CC=gcc
CFLAGS=-Wall -D_GNU_SOURCE -O
LD_LIBS=-lcrypto
SOURCE=encrypt.c
OBJ=$(SOURCE:.c=.o)

all: $(OBJ)

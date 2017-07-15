CC=g++
CXXFLAGS=-Wall -D_GNU_SOURCE $(shell pkg-config --cflags libbitcoin-client)
LDLIBS=-lcrypto $(shell pkg-config --libs libbitcoin-client)
SOURCE=$(wildcard *.cpp)
OBJ=$(SOURCE:.cpp=.o)
EXECUTABLES=ransom addressServer walletMenu

Debug: CXXFLAGS += -Og -g

Release: CXXFLAGS += -O

.PHONY: Release Debug clean

Debug: $(EXECUTABLES)

Release: $(EXECUTABLES)

addressServer: addressServer.o wallet.o

walletMenu: walletMenu.o wallet.o

ransom: encrypt.o ransom.o

clean:
	rm -f $(OBJ) $(EXECUTABLES)

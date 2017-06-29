CC=g++
CXXFLAGS=-Wall -D_GNU_SOURCE 
LDLIBS=-lcrypto
SOURCE=encrypt.cpp main.cpp
OBJ=$(SOURCE:.cpp=.o)
EXECUTABLE=ransom.out

Debug: CXXFLAGS += -Og -g

Release: CXXFLAGS += -O

.PHONY: Release Debug clean

Release: $(EXECUTABLE)

Debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LOADLIBS) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

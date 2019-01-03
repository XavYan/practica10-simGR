IDIR =../include
CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -g

ODIR=../src
LDIR =../lib

LIBS=-lm

_OBJ = main.o GR.o DFA.o state_t.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: GR

$(ODIR)/main.o: $(ODIR)/main.cpp $(ODIR)/GR.cpp $(ODIR)/DFA.cpp $(ODIR)/state_t.cpp $(IDIR)/GR.hpp $(IDIR)/DFA.hpp $(IDIR)/state_t.hpp
	$(CC) -c -o $@ $< $(CFLAGS)

GR: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(INCDIR)/*~

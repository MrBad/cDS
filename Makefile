DFLAGS=
INCLUDE=.
LIBS=
CC=gcc
OFLAGS=-c
CFLAGS=-g3 -Wall -Wextra -std=c99 -pedantic -I$(INCLUDE) $(DFLAGS)
MAKE=make

TARGET=main
OBJECTS=list.o tree.o

all: $(TARGET) tester test

$(TARGET): $(OBJECTS) main.o Makefile
	$(CC) $(CFLAGS) $(DFLAGS) -o $(TARGET) main.o $(OBJECTS) $(LIBS)

%o: %.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

run: $(TARGET) *.o *.c *.h
	./$(TARGET)

clean:
	rm -f main.o $(OBJECTS) $(TARGET) tester tester.o _main.o

tester: tester.o main.o $(OBJECTS)
	strip -N main main.o -o _main.o
	$(CC) -o $@ tester.o _main.o $(OBJECTS) $(LDFLAGS)

test:
	./tester


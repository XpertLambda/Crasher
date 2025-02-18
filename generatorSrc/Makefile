CC = gcc
CFLAGS = -O2 -Wall -Wextra -pedantic
SRC = src

all: wordlist_generator

wordlist_generator: main.o wordlist.o
	$(CC) $(CFLAGS) -o wordlist_generator main.o wordlist.o

main.o: $(SRC)/main.c $(SRC)/wordlist.h
	$(CC) $(CFLAGS) -c $(SRC)/main.c

wordlist.o: $(SRC)/wordlist.c $(SRC)/wordlist.h
	$(CC) $(CFLAGS) -c $(SRC)/wordlist.c

clean:
	rm -f *.o wordlist_generator

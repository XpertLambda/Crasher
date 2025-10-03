CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O3 -fopenmp
SRCDIR = src
TARGET = crasher

SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)
LIBS = -lssl -lcrypto -lpthread -lm
INCLUDES = -I$(SRCDIR)

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LIBS) -o $@

clean:
	rm -f $(TARGET)

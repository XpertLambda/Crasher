CC = gcc
CFLAGS = -Wall -Wextra -O2 -lcrypto -fopenmp

# Directories
CRASHER_SRC_DIR = crasherSrc/src
DEHASH_DIR = $(CRASHER_SRC_DIR)/DeHash
HASH_TYPES_DIR = $(DEHASH_DIR)/HashTypes
OPTIONS_DIR = $(DEHASH_DIR)/Options
LOGO_DIR = $(CRASHER_SRC_DIR)/Logo

GENERATOR_SRC_DIR = generatorSrc/src

# Source files for Crasher
CRASHER_SRC_FILES = $(CRASHER_SRC_DIR)/Main.c \
                    $(DEHASH_DIR)/DeHash.c \
                    $(HASH_TYPES_DIR)/md5Hash.c \
                    $(HASH_TYPES_DIR)/SHA1Hash.c \
                    $(HASH_TYPES_DIR)/SHA256Hash.c \
                    $(OPTIONS_DIR)/ASCII.c \
                    $(OPTIONS_DIR)/Randomlist.c \
                    $(OPTIONS_DIR)/Wordlist.c \
                    $(LOGO_DIR)/Logo.c

# Source files for Wordlist Generator
GENERATOR_SRC_FILES = $(GENERATOR_SRC_DIR)/main.c \
                      $(GENERATOR_SRC_DIR)/wordlist.c

# Object files for Crasher
CRASHER_OBJ_FILES = $(CRASHER_SRC_FILES:.c=.o)

# Object files for Wordlist Generator
GENERATOR_OBJ_FILES = $(GENERATOR_SRC_FILES:.c=.o)

# Targets
CRASHER_TARGET = crasher
GENERATOR_TARGET = generator

all: $(CRASHER_TARGET) $(GENERATOR_TARGET)

# Build Crasher
$(CRASHER_TARGET): $(CRASHER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(CRASHER_TARGET) $(CRASHER_OBJ_FILES)

# Build Wordlist Generator
$(GENERATOR_TARGET): $(GENERATOR_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(GENERATOR_TARGET) $(GENERATOR_OBJ_FILES)

# Compile Crasher object files
%.o: $(CRASHER_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Wordlist Generator object files
%.o: $(GENERATOR_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CRASHER_OBJ_FILES) $(CRASHER_TARGET)
	rm -f $(GENERATOR_OBJ_FILES) $(GENERATOR_TARGET)

.PHONY: all clean

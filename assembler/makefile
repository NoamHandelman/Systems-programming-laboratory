# Compiler and flags
CC = gcc
CFLAGS = -ansi -Wall -pedantic

# Directories
SRC_DIR = .
UTILS_DIR = utils
HEADERS_DIR = headers

# Source and Object files
SRC_FILES = assembler.c pre_proc.c $(UTILS_DIR)/memory.c $(UTILS_DIR)/strings.c $(UTILS_DIR)/lexer.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Target executable
TARGET = assembler

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.o
	rm -f $(UTILS_DIR)/*.o

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies
assembler.o: $(HEADERS_DIR)/pre_proc.h
pre_proc.o: $(HEADERS_DIR)/data_struct.h $(HEADERS_DIR)/globals.h $(HEADERS_DIR)/pre_proc.h $(HEADERS_DIR)/memory.h $(HEADERS_DIR)/strings.h $(HEADERS_DIR)/lexer.h
$(UTILS_DIR)/memory.o: $(HEADERS_DIR)/memory.h
$(UTILS_DIR)/strings.o: $(HEADERS_DIR)/strings.h
$(UTILS_DIR)/lexer.o: $(HEADERS_DIR)/globals.h $(HEADERS_DIR)/data_struct.h


# Clean target
clean:
	rm -f $(TARGET) $(OBJ_FILES) $(UTILS_DIR)/*.o

.PHONY: all clean

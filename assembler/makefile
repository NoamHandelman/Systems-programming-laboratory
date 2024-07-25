CC = gcc
CFLAGS = -ansi -Wall -pedantic

SRC_DIR = .
UTILS_DIR = utils
HEADERS_DIR = headers

SRC_FILES = assembler.c pre_proc.c assembler_transitions.c $(UTILS_DIR)/memory.c $(UTILS_DIR)/strings.c $(UTILS_DIR)/lexer.c $(UTILS_DIR)/files.c $(UTILS_DIR)/data_struct.c $(UTILS_DIR)/code_conversions.c
OBJ_FILES = $(SRC_FILES:.c=.o)

TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.o
	rm -f $(UTILS_DIR)/*.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

assembler.o: $(HEADERS_DIR)/pre_proc.h $(HEADERS_DIR)/assembler_transitions.h
pre_proc.o: $(HEADERS_DIR)/data_struct.h $(HEADERS_DIR)/globals.h $(HEADERS_DIR)/pre_proc.h $(HEADERS_DIR)/memory.h $(HEADERS_DIR)/strings.h $(HEADERS_DIR)/lexer.h $(HEADERS_DIR)/files.h
$(UTILS_DIR)/memory.o: $(HEADERS_DIR)/memory.h
$(UTILS_DIR)/strings.o: $(HEADERS_DIR)/strings.h
$(UTILS_DIR)/lexer.o: $(HEADERS_DIR)/globals.h $(HEADERS_DIR)/data_struct.h
$(UTILS_DIR)/files.o: $(HEADERS_DIR)/files.h $(HEADERS_DIR)/memory.h
$(UTILS_DIR)/code_conversions.o: $(HEADERS_DIR)/data_struct.h
assembler_transitions.o: $(HEADERS_DIR)/assembler_transitions.h $(HEADERS_DIR)/code_conversions.h 

clean:
	rm -f $(TARGET) $(OBJ_FILES) $(UTILS_DIR)/*.o

.PHONY: all clean

assembler: assembler.o pre_proc.o utils/memory_helper.o
	gcc -ansi -Wall -pedantic assembler.o pre_proc.o utils/memory_helper.o -o assembler
	rm -f *.o
	rm -f utils/*.o

assembler.o: assembler.c headers/pre_proc.h
	gcc -ansi -Wall -pedantic -c assembler.c -o assembler.o

pre_proc.o: pre_proc.c headers/data_struct.h headers/globals.h headers/pre_proc.h headers/memory_helper.h
	gcc -ansi -Wall -pedantic -c pre_proc.c -o pre_proc.o

utils/memory_helper.o: utils/memory_helper.c headers/memory_helper.h
	gcc -ansi -Wall -pedantic -c utils/memory_helper.c -o utils/memory_helper.o

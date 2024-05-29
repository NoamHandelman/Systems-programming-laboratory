myset: myset.o set.o
	gcc -ansi -Wall -pedantic myset.o set.o -o myset
	rm -f *.o

myset.o: myset.c set.h
	gcc -ansi -Wall -pedantic -c myset.c -o myset.o

set.o: set.c set.h
	gcc -ansi -Wall -pedantic -c set.c -o set.o

myset: main.o myset.o
	gcc -ansi -Wall -pedantic main.o myset.o -o myset
	rm -f *.o

main.o: main.c set.h
	gcc -ansi -Wall -pedantic -c main.c -o main.o

myset.o: myset.c set.h
	gcc -ansi -Wall -pedantic -c myset.c -o myset.o

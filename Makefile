main: main.o api.o
	gcc -o main main.o api.o -lm -g

main.o: main.c api.h
	gcc -c main.c -Wall -ansi -pedantic -o main.o -lm -g

api.o: api.c api.h
	gcc -c api.c -Wall -ansi -pedantic -o api.o -lm -g

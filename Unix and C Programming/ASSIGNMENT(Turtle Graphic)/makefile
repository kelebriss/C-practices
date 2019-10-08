CFLAGS = -Wall -ansi -pedantic -g -w

main : main.o readfile.o process.o effects.o writefile.o
	gcc main.o readfile.o process.o effects.o writefile.o -o main -lm

main.o : main.c readfile.h effects.h process.h writefile.h
	gcc -c main.c $(CFLAGS)

readfile.o : readfile.c readfile.h 
	gcc -c readfile.c $(CFLAGS)

writefile.o : writefile.c readfile.h writefile.h
	gcc -c writefile.c $(CFLAGS)

process.o : process.c readfile.h effects.h process.h writefile.h
	gcc -c process.c $(CFLAGS)

effects.o : effects.c effects.h
	gcc -c effects.c $(CFLAGS)



clean :
	rm -f main main.o readfile.o process.o writefile.o effects.o

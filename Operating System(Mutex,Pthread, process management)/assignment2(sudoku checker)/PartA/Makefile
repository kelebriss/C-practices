CC = gcc
CFLAGS = -g -Wall -pedantic -ansi 
OBJ = mssv.o readFile.o 
EXEC = mssv

$(EXEC) : $(OBJ)
	$(CC) -g -o $(EXEC) -lm -pthread -lrt $(OBJ) 
    
mssv.o : mssv.c readFile.h 
	$(CC) -std=gnu99 -c mssv.c 

readFile.o : readFile.c readFile.h 
	$(CC) $(CFLAGS) -c readFile.c  

clean :
	rm -f $(EXEC) $(OBJ)
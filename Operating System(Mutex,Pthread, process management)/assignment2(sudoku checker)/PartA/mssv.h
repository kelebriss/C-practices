#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "readFile.h"
/* To check is that 9 number not repeat */
/* In this case is for each row, box and column */
int checkNineNumber(int* nineNumber);

typedef struct 
{
    int** answers;
} Buffer1;

typedef struct 
{
    int* answers;
} Buffer2;

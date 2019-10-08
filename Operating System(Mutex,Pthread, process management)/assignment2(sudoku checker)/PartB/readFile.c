#include "readFile.h"

/*To read the sudoku solution file */
int readFile(int**sudoku, char *filename)
{
	FILE *f;
    int isFinish = 0; /* 0 is false */
    int ii, jj;
    int number;
    int isValid;

	f = fopen(filename, "r");

	if(f == NULL)
		perror(filename);
	else
	{
		do
		{
			for(ii = 0; ii < 9; ii++)
			{
				for(jj = 0; jj < 9; jj++)
				{
					isValid = fscanf(f, "%d", &number);
                    
					if(isValid == EOF)
						isFinish = 1;
					else
						sudoku[ii][jj] =  number;
				}
			}
		}
		while(!isFinish);
	}

	fclose(f);
    
    return isFinish;
}


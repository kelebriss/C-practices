#include "mssv.h"

/* This main is using multiple process to make Sudoku Solution Validator */
/* The Sudoku Solution Validator determines if a solution to the Sudoku puzzle is valid.*/
/* Making 0~8th children processes to do the row check, 
   9th child is to check columns and 10th child is to check sub-grids */
   
/*Since the timing of write file, write counter and write buffer2 is all the same,
  so I just use one variable to control it*/ 
  
int main(int argc, char *argv[])
{
    Buffer1* buffer1;
    int* buffer2;
    pid_t pid;
    int* counter;

    /* For blocking (up to 11) */
    int* isFinishValidation;
    int* counterTurn; 
    
    /* For blocking boolean*/
    int* counterIsWriteable;

    
    /*local variable*/
    int ii, jj, kk; 
    int numOfValid = 0;
    char* numberOfInvalid = (char*) malloc( 256 * sizeof(char));
    int isNumberOfInvalidUsed = 0;
    
    /*local variable for one row , column or sub-grid*/
    int* localArray = (int*) malloc(9 * sizeof(int));
    /* This is boolean for identify is which block is valid for sudoku */
    int* isValid = (int*) malloc(9 * sizeof(int)); 

    /* This is a boolean, for identify is itself parent */
    int isParent = 1; 
    int childrenNo = 11;
    
    
    buffer1 = mmap(NULL, sizeof(Buffer1), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    buffer1->answers = mmap(NULL,9*sizeof(int*),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(ii = 0; ii < 9; ii++) 
    {
        buffer1->answers[ii] = mmap(NULL,9*sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    }
    buffer2 = mmap(NULL, 11*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    isFinishValidation = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    counterIsWriteable = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    counterTurn = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    /*init*/
    (*counter) = 0; 
    (*counterTurn) = -1; /*init to no one turn*/
    
    (*isFinishValidation) = 0; 
    (*counterIsWriteable) = 1;
    
    for(ii = 0; ii <9; ii++)
    {
        isValid[ii] = 0;
    }
    
    readFile(buffer1->answers,argv[1]);
    
    
    /*11 children process */
    pid = fork();
    if(pid == 0)
    {
        isParent = 0;
        childrenNo--;
    }
    for(ii = 0; ii< 10; ii++)
    {
        /*just last child increase the process*/
        if(pid == 0) 
        {
            pid = fork();
            if(pid == 0)
            {
                isParent = 0;
                childrenNo--;
            }
        }
    }    
    /*Finish 11 children */
    
    if(isParent != 0)
    {
        /*block Parent waitting for children to end*/
        while((*isFinishValidation) < 11) { msync( isFinishValidation, sizeof(int) , MS_SYNC ); }
        if( (*counter) < 27) 
        {
            printf("There are %d valid sub-grids.\n", (*counter));
        }
        else
        {
            printf("There are 27 valid sub-grids, and thus the solution is valid.\n");
        }
        writeFile(-1, numberOfInvalid);
        
        /*free the memory*/
        free(numberOfInvalid);
        free(localArray);
        free(isValid);
        for(ii = 0; ii < 9; ii++) 
        {
            munmap(buffer1->answers[ii], 9*sizeof(int));
        }
        munmap(buffer1->answers, 9*sizeof(int*));
        munmap(buffer1, sizeof(Buffer1));
        munmap(buffer2, 11*sizeof(int));
        munmap(counter, sizeof(int));
        munmap(isFinishValidation, sizeof(int));
        munmap(counterIsWriteable, sizeof(int));
        munmap(counterTurn, sizeof(int));
        
    }
    else
    {
        sleep(atoi(argv[2]));
        if(childrenNo < 9)
        {
            for(ii = 0; ii < 9; ii++)
            {
                localArray[ii] = buffer1->answers[childrenNo][ii];
            }
            
            isValid[childrenNo] = checkNineNumber(localArray);
            
            if(isValid[childrenNo])
            {
                numOfValid++;
                printf("Validation result from process ID-%d: row %d is valid\n", pid ,childrenNo + 1);
            }
            else
            {
                if(isNumberOfInvalidUsed == 0)
                {
                    sprintf(numberOfInvalid, " %d", ii + 1);
                    isNumberOfInvalidUsed = 1;
                }
                else
                {
                    sprintf(numberOfInvalid, "%s, %d",numberOfInvalid , ii + 1);
                }
            }
            
            
            /* Try to write counter*/
            while(!(*counterIsWriteable)) { msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); }
            
            (*counterIsWriteable) = 0; /*lock other process*/
            (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
            
            /* while can write but not your turn,
               then should change it to not writeable and change the turn to yourself 
            */
            while( (*counterIsWriteable) || ((*counterTurn) != childrenNo) ) 
            { 
                (*counterIsWriteable) = 0; /*lock other process*/
                (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
                msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); 
                msync( counterTurn, sizeof(int*) , MS_SYNC ); 
            }
            (*counter) = (*counter) + numOfValid;
            if(!isValid[childrenNo])
            {
                writeFile(childrenNo, numberOfInvalid);
            }
            
            buffer2[childrenNo] = numOfValid;
            (*isFinishValidation)++;
            
            
            (*counterIsWriteable) = 1;
            
        }
        else if(childrenNo == 9)
        {
            for(ii = 0; ii < 9; ii++)
            {
                for(jj = 0; jj < 9; jj++)
                {
                    localArray[jj] = buffer1->answers[jj][ii];
                }
                isValid[ii] = checkNineNumber(localArray);
                if(isValid[ii])
                {
                    numOfValid++;
                }
                else
                {
                    if(isNumberOfInvalidUsed == 0)
                    {
                        sprintf(numberOfInvalid, " %d", ii + 1);
                        isNumberOfInvalidUsed = 1;
                    }
                    else
                    {
                        sprintf(numberOfInvalid, "%s, %d",numberOfInvalid , ii + 1);
                    }
                }
            }
            
            /* Try to write counter*/
            while(!(*counterIsWriteable)) { msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); }
            
            (*counterIsWriteable) = 0; /*lock other process*/
            (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
            
            /* while can write but not your turn,
               then should change it to not writeable and change the turn to yourself 
            */
            while( (*counterIsWriteable) || ((*counterTurn) != childrenNo) ) 
            { 
                (*counterIsWriteable) = 0; /*lock other process*/
                (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
                msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); 
                msync( counterTurn, sizeof(int*) , MS_SYNC ); 
            }
            (*counter) = (*counter) + numOfValid;
            
            if(isNumberOfInvalidUsed)
            {
                writeFile(childrenNo, numberOfInvalid);
            }
            
            printf("Validation result from process ID-%d: %d of 9 columns are valid\n", pid, numOfValid);
            
            buffer2[childrenNo] = numOfValid;
            (*isFinishValidation)++;
            
            (*counterIsWriteable) = 1;
        }
        else if(childrenNo == 10)
        {
            for(ii = 0; ii < 9; ii++)
            {
                for(jj = 0; jj < 3; jj++)
                {
                    if( ii < 3 )
                    {                              
                        for(kk = 0; kk < 3; kk++)
                        {
                            localArray[kk+jj*3] = buffer1->answers[jj][kk+ii*3];
                        }
                    }
                    else if( ii < 6 )
                    {
                        for(kk = 0; kk < 3; kk++)
                        {
                            localArray[kk+jj*3] = buffer1->answers[jj+3][kk+ (ii - 3) *3];
                        }
                    }
                    else
                    {
                        for(kk = 0; kk < 3; kk++)
                        {
                            localArray[kk+jj*3] = buffer1->answers[jj+6][kk + (ii - 6) * 3];
                        }
                    }
                }

                isValid[ii] = checkNineNumber(localArray);
                if(isValid[ii])
                {
                    numOfValid++;
                }
                else
                {
                    if(isNumberOfInvalidUsed == 0)
                    {
                        sprintf(numberOfInvalid, " %d", ii+ 1);
                        isNumberOfInvalidUsed = 1;
                    }
                    else
                    {
                        sprintf(numberOfInvalid, "%s, %d",numberOfInvalid , ii + 1);
                        //strcat(numberOfInvalid, ", " + ii + 1);
                    }
                }
            }
            
            /* Try to write counter*/
            while(!(*counterIsWriteable)) { msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); }
            
            (*counterIsWriteable) = 0; /*lock other process*/
            (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
            
            /* while can write but not your turn,
               then should change it to not writeable and change the turn to yourself 
            */
            while( (*counterIsWriteable) || ((*counterTurn) != childrenNo) ) 
            { 
                (*counterIsWriteable) = 0; /*lock other process*/
                (*counterTurn) = childrenNo; /*make the counterTurn to your turn*/
                msync( counterIsWriteable, sizeof(int*) , MS_SYNC ); 
                msync( counterTurn, sizeof(int*) , MS_SYNC ); 
            }
            (*counter) = (*counter) + numOfValid;
            
            if(isNumberOfInvalidUsed)
            {
                writeFile(childrenNo, numberOfInvalid);
            }
            
            buffer2[childrenNo] = numOfValid;
            (*isFinishValidation)++;
            
            (*counterIsWriteable) = 1;
            
            printf("Validation result from process ID-%d: %d of 9 sub-grids are valid\n",pid, numOfValid);
        }
        exit(0);
    }
    
	return 0;
}

/* To check is that 9 number not repeat */
/* In this case is for each row, box and column */
int checkNineNumber(int* nineNumber)
{
    /* This is a boolean, init to true*/
	int isValid = 1; 
    
    int ii;
    /*These are boolean for check is each number have only one*/
    int valid1, valid2, valid3, valid4, valid5, valid6, valid7, valid8, valid9;
    
    /*init them*/
    valid1 = 0;
    valid2 = 0;
    valid3 = 0;
    valid4 = 0;
    valid5 = 0;
    valid6 = 0;
    valid7 = 0;
    valid8 = 0;
    valid9 = 0;
    
    for(ii = 0; ii < 9; ii++)
    {
        switch(nineNumber[ii])
        {
            case 1:
                if(valid1 == 0)
                    valid1 = 1;
                else
                    isValid = 0;
                break;
            case 2:
                if(valid2 == 0)
                    valid2 = 1;
                else
                    isValid = 0;
                break;
            case 3:
                if(valid3 == 0)
                    valid3 = 1;
                else
                    isValid = 0;
                break;
            case 4:
                if(valid4 == 0)
                    valid4 = 1;
                else
                    isValid = 0;
                break;
            case 5:
                if(valid5 == 0)
                    valid5 = 1;
                else
                    isValid = 0;
                break;
            case 6:
                if(valid6 == 0)
                    valid6 = 1;
                else
                    isValid = 0;
                break;
            case 7:
                if(valid7 == 0)
                    valid7 = 1;
                else
                    isValid = 0;
                break;
            case 8:
                if(valid8 == 0)
                    valid8 = 1;
                else
                    isValid = 0;
                break;
            case 9:
                if(valid9 == 0)
                    valid9 = 1;
                else
                    isValid = 0;
                break;
            default:
                isValid = 0;
        }
    }

	return isValid;	
}

/*To write log file */
int writeFile(int childrenNo, char* numberOfInvalid)
{
	FILE *f;
    int isFinish = 0; /* 0 is false */
    int ii, jj;
    int number;
    int isValid;

	f = fopen("log", "a");

	if(f == NULL)
		perror("log");
	else
	{
        if( childrenNo == -1)
        {
            /*For knowing when is one program ended*/
            fprintf(f,"--------------------------------------\n");
        }
        else if(childrenNo < 9)
        {
            fprintf(f,"process ID-%d: row %d is invalid\n", childrenNo+1, childrenNo + 1);
        }
        else if( childrenNo == 9)
        {
            fprintf(f,"process ID-%d: column%s is invalid\n", childrenNo + 1, numberOfInvalid);
        }
        else if( childrenNo == 10)
        {
            fprintf(f,"process ID-%d: sub-grid%s is invalid\n", childrenNo + 1, numberOfInvalid);
        }
		
	}

	fclose(f);
    
    return isFinish;
}






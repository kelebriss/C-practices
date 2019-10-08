/*
 * This file is the main function which call all of process.
 * 
 * Author: Hyojin Cha(kevin) 90008547
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"
#include "writefile.h"
#include "process.h"
#include "effects.h"

int main(int argc, char* argv[])
{
	
	dataList* d = (dataList*)calloc(1,sizeof(dataList)); /* allocate the list for storing the input value*/
	outputList* o = (outputList*)calloc(1,sizeof(outputList)); /* allocate the list for writing the output value*/
	if(argv[1] == NULL) /*if input is wrong then display this*/
	 printf("\nyou entered wrong file nmae. please enter the correct file name\n\n");
	else
	{
	 clearScreen(); /*before processing, make terminal screen clear*/
	 readFile(argv[1], d); /*storing the value into linked list from entered file input*/
	 process(d, o); /* drawing the line on the screen and storing the value to linked list for output log data*/
	 writefile("graphics.log", o); /*to write into file which named 'graphic.log'*/
 	 penDown(); /*make sure command line going bottom line for to look better*/
	}
	freeInputList(d); /*deallocate the list*/
	freeOutputList(o); /*deallocate the list*/
return 0;


}

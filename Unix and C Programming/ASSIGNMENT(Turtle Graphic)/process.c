/*
 * This file is the processing function which doing move or draw with set value,

 *
 * Author: Hyojin Cha(kevin) 90008547
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "readfile.h"
#include "effects.h"
#include "writefile.h"
#include "process.h"


void process(dataList* list, outputList* output)
{
	double x=0, y=0,cPosx=0, cPosy=0, cAng=0, length,rad; /*set initial values*/
	int cFG=7, cBG=0; /*set initial values*/
	double cx,cy,nx,ny;
	char cPat = '+'; /*set initial value*/
	PlotFunc plotter;
	iData* data;
	void* p = (void*)malloc(sizeof(char)); /*allocate void pointer for input values*/

	plotter = &plotting; /*get function plotting(below this function)*/
	data = list->head; /*make the cursor and point the head*/

	while(data != NULL)
	{
	 if(strcmp(data->type,"ROTATE")==0) /*if type in linked list is ROTATE then replace the angle*/
	 {
	  cAng += data->realValue;
	 }
	 else if(strcmp(data->type,"MOVE")==0) /*if type in linked list is MOVE then to move to new coodinate from current coordinate*/
	 {
	  if(data->realValue >0.0)
	  {
	   length = data->realValue; 
	   rad =  cAng/57.295; /*trigonometry function takes radius value so tranfer it*/
	   x = (length * cos(rad));
	   y = -(length * sin(rad)); /*terminal screen is up side down so y-coordinate need to multiply -*/
	   *(char*)p = ' '; /*MOVE is not drawing any line so put blank pattern*/

	   cx = round(cPosx); /*do round the value(the function exist below this function)*/
	   cy = round(cPosy);
	   nx = round(cPosx+x);
	   ny = round(cPosy+y);
	
	   if(nx<0 || ny<0)/*terminal can not display negative coordinate point*/
	    printf(" coordinate point can not be negative value");
	   else
	   {
	    line((int)cx,(int)cy,(int)nx,(int)ny, plotter,p); /*pass double value with int type cast because this function required integer*/
 	    recording(output,data->type, cx, cy, nx, ny);
   	    cPosx += x; /*change the current position*/
	    cPosy += y;
	   }
	  }
	  else /*if value is negative print this*/
	   printf("length must be positive real value");
	 }
	 else if(strcmp(data->type,"DRAW")==0) /*if type in linked list is DRAW then to DRAW to new coodinate from current coordinate*/
	 {
	  if(data->realValue >0.0)
	  {

	   length = data->realValue;
	   rad =  cAng/57.295;/*trigonometry function takes radius value so tranfer it*/
	   x = length * cos(rad);
	   y = -(length * sin(rad));/*terminal screen is up side down so y-coordinate need to multiply -*/
	   *(char*)p = cPat; /*taking the set pattern*/

	   cx = round(cPosx);/*do round the value(the function exist below this function)*/
	   cy = round(cPosy);
	   nx = round(cPosx+x);
	   ny = round(cPosy+y);

	   if(nx<0 || ny<0) /*terminal can not display negative coordinate point*/
	    printf(" coordinate point can not be negative value");
	   else
	   {
	    line((int)cx,(int)cy,(int)nx,(int)ny, plotter,p); /*pass double value with int type cast because this function required integer*/
 	    recording(output,data->type, cx, cy, nx, ny);
   	    cPosx += x; /*change the current position*/
	    cPosy += y;
	   }
	  }
	  else/*if value is negative print this*/
	   perror("length must be positive real value");
	 }
	 else if(strcmp(data->type,"FG")==0)/*if type in linked list is FG then changing the foreground color by entered number*/
	 {
	  if(data->integerValue > 0 && data->integerValue < 16)
	  {
	   cFG = data->integerValue;
	   setFgColour(data->integerValue);
	  }
	  else /*if number is out of range then print this*/
	   printf("number must be between 0 and 15");
	 }
	 else if(strcmp(data->type,"BG")==0)/*if type in linked list is BG then changing the background color by entered number*/
	 {
	  if(data->integerValue > 0 && data->integerValue < 8)
	  {
	   cBG = data->integerValue;
	   setBgColour(data->integerValue);
	  }
	  else/*if number is out of range then print this*/
	   printf("number must be between 0 and 7");
	 }
	 else  /*set the PATTERN as current pattern*/
	 {
	  cPat = data->pat;
	 }
	 data = data->next; /*move to next node*/
	}
	free(p); /*free the void pointer*/
	p = NULL;
}
void plotting (void *data) /*plotting function whicj take parameter void pointer then print out this void pointer with type cast*/
{
	printf("%c\n",*(char*)data);
}
double round(double a) /*round function*/
{
	a = floor(a+0.5);
	return a;
}

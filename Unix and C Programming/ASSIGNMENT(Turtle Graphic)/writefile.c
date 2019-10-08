/*
 * This file is the writefile function which writing the output file from linked list.
 * 
 * Author: Hyojin Cha(kevin) 90008547
 */

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "readfile.h"
#include "writefile.h"

void writefile(char* wFile, outputList* output)

{
	FILE* f=fopen(wFile, "a"); /*open the file with append option*/
	if(f ==NULL)
	 perror("can not open the file");
	else
	{
	 oData* current = output->head;
	 fputs("---\n",f); /*write into the file for separate the new and old output*/
	 while(current != NULL) 
	 {
	  fprintf(f, " %s ( %6.3f, %6.3f)-( %6.3f, %6.3f)\n",current->type, current->cx, current->cy, current->nx, current->ny); /*write into the file the coodinate and type(move or draw)*/
	  current = current->next;
	 }
	 fclose(f); /*close file*/
	}
}

void recording(outputList* output,char* type, double cox, double coy, double nox, double noy)
{
	int i;
	oData *current, *node;
	char *tmp;

	i=strlen(type);
	tmp = (char*)calloc(i+1,sizeof(char));/*allocate the String, +1 reason is (ward + \0) so need to add 1*/
	node = (oData*)calloc(1,sizeof(oData)); /*allocate the node for insert into linked list */
	current = output->head; /*it is for cursor*/
	strcpy(tmp,type); /*copy the string parameter type(word) to tmp */

	if(current ==NULL) /*if linked list is null then do this*/
	{
	 output->head = node;
	}	
	else
	{
	 while(current->next != NULL) /*going end of linked list because end-insert linked list*/
	 {
	  current = current->next;
	 }
	  current->next= node;
	}
	strcpy(node->type,tmp); /*copy the string tmp to linked list*/
	node->next = NULL; /*set the node next pointing NULL*/
	node->cx = cox; /*set the coordinate current x,y and new x,y*/
	node->cy = coy; 
	node->nx = nox; 
	node->ny = noy; 

	free(tmp); /*free the String*/
	tmp = NULL;
}

void freeOutputList(outputList* list) /*freed linked list function*/
{
	oData *data, *nextData;
	data = (*list).head;
	while(data != NULL) {
	nextData = (*data).next;
	free(data);
	data = nextData;
}
free(list);
}
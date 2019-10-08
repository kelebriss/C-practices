/*
 * This file is the readfile function which read the file and save the data into linked list.
 * 
 * Author: Hyojin Cha(kevin) 90008547
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"
void readFile(char* rFile, dataList* list)
{
	char var1[8], var2;
	int var3, count;
	double var4;
	void *data = (void*)calloc(1,sizeof(double));	

	FILE* f = fopen(rFile,"r"); /* open the file with read option*/
	if(f==NULL)
	 perror("can not open the file");
	else
	{ 
	fscanf(f,"%s", var1); /* get the first word */

	do{	
	 upper(var1);/* make all of character in word to be upper charater for to compare easier*/
	 if(strcmp(var1,"ROTATE")==0 || strcmp(var1,"DRAW")==0 || strcmp(var1,"MOVE")==0) /*if word is ROTATE,DRAW or MOVE then scan real value and save on void pointer*/
	 {
	   fscanf(f,"%lf", &var4);
	   *(double*)data = var4;
	 }
	 else if(strcmp(var1,"FG")==0 || strcmp(var1,"BG")==0) /*if word is FG or BG then scan integer value and save on void pointer*/
	 {
	   fscanf(f,"%d", &var3);
	   *(int*)data = var3;
	 }
	 else if(strcmp(var1,"PATTERN")==0) 
	 {
	   fscanf(f," %c", &var2);
	   *(char*)data = var2;
	 }
	 else /*else all the other word is incorrect so print message*/
	  printf("invalid type of name");

	 insert(list, var1, data); /*pass to insert function then store into linked list*/
	 count = fscanf(f,"%s", var1); /*check again next line first word in file and count for do-while condition*/
	 
	}while(count==1); /*do-while if count is 1 then read something so do it again if 0 then quit*/
	fclose(f); /*close the file*/
	}
	free (data); /*free void pointer*/
	data = NULL;
	
}

void insert(dataList* l,char* name, void* data)
{
	int i;
	iData *current, *node;
	char *tmp; 
	
	i=strlen(name);	
	tmp = (char*)calloc(i+1,sizeof(char));/*allocate the String, +1 reason is (ward + \0) so need to add 1*/
	node = (iData*)calloc(1,sizeof(iData)); /*allocate the node for inserting*/
	current = l->head; /*it is for cursor*/
	strcpy(tmp,name); /*copy the string parameter name(word) to tmp */

	if(current == NULL)/*if linked list is null then do this*/
	{
	 l->head = node;
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

	if(strcmp(name,"ROTATE")==0 || strcmp(name,"DRAW")==0 || strcmp(name,"MOVE")==0) /*if name is ROTATE,DRAW or MOVE then save value with typecast double then into linked list*/
	 node->realValue = *(double*)data; 
	else if(strcmp(name,"FG")==0 || strcmp(name,"BG")==0) /*if name is BG or FG then save value with typecast integer then into linked list*/
	 node->integerValue = *(int*)data;
	else /*else save value with typecast char then into linked list*/
	 node->pat = *(char*)data;

	free(tmp); /*free the String*/
	tmp = NULL;
}
void upper(char var[]) 
{
	int i,j;
	j=strlen(var);
	char *cha = (char*)calloc(j+1,sizeof(char)); /*allocate the String, +1 reason is (ward + \0) so need to add 1*/
	
	for(i=0; i < j; i++)
	{
	 if(var[i]>= 'a' && var[i] <= 'z')/*between a to z then do this*/
	  cha[i] = var[i]-32; /*upper's and lower's ASCII code different is 32 so (lower-32) is upper character*/
	 else
	  cha[i] = var[i]; /*dont need to change then keep it*/
	
	}
	strcpy(var, cha); /*String cha copy into var*/
	free(cha);
	cha = NULL;
}

void freeInputList(dataList* list) /*freed linked list function*/
{
	iData *data, *nextData;
	data = (*list).head;
	while(data != NULL) {
	nextData = (*data).next;
	free(data);
	data = nextData;
}
free(list);
}

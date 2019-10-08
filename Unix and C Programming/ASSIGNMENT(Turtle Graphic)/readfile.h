#ifndef __readfile_h__
#define __refile_h__

typedef struct iData{ /* linked list for having input data*/
	char type[8]; /*longest word is 7character(PATTERN) and \0 so need to set 8character String*/
	double realValue;
	int integerValue;
	char pat;
	struct iData* next;
}iData;

typedef struct
{
	iData* head;
}dataList;
void readFile(char* rFile, dataList* list); /*readfile and distribute the data and values*/
void insert(dataList* l,char* name, void* data); /*taking the value from file and inserting into linked list*/
void freeInputList(dataList* list) ; /*free the allocate list and node*/
void upper(char var[]); /*make any lower case character in word to make into upper case character*/
#endif

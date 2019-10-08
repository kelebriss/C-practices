#ifndef __writefile_h__
#define __writefile_h__

typedef struct oData{ /*linked list for having output data*/
	char type[7];
	double cx;
	double cy;
	double nx;
	double ny;
	struct oData* next;
}oData;

typedef struct
{
	oData* head;
}outputList;

void writefile(char* wFile,outputList* output);  /*open the file and call recording() for writing the data*/
void recording(outputList* output,char* type, double cx, double cy, double nx, double ny); /*taking the value from linked list and inserting into filet*/
void freeOutputList(outputList* list) ; /*free the allocate list and node*/
#endif
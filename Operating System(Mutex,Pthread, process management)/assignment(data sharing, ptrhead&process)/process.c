//////////////////////////헤더파일/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>       
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
/////////////////////전역변수 선언//////////////////////////////////
#define db_size 5 //data buffer size
int readcount;//this is using as flag in this program
//int *db=NULL; // data bufer for testing
int t1,t2,shmid=5;
sem_t wrt,mutex;       // wrt와mutex의 semaphore선언
FILE *file1;
//////////////////////////////////////////////////////////////////
void creat_process(int rnum,int wnum);
int initialize();

int main(int argc, char *argv[])
{

	int rdrn,wrtn;//number of reder and writer
	int i,j;
	rdrn = atoi(argv[1]);
	wrtn = atoi(argv[2]);
	t1 = atoi(argv[3]);
	t2 = atoi(argv[4]);
	initialize();
	file1=fopen("data","r");
	creat_process(rdrn,wrtn);

    	sem_destroy(&wrt);// 초기화된것을 write삭제
    	sem_destroy(&mutex);// 초기화된것을 mutex삭제
	fclose(file1);
	return 0;
}

int initialize()
{
 sem_init(&wrt,1,1);//wrt 초기화
 sem_init(&mutex,1,1);//mutex 초기화
 return 0;
}
void creat_process(int rnum, int wnum)
{
	pid_t r, w, *rchildPids=NULL, *wchildPids=NULL;
	int i,j,rwaiting,wwaiting; // tc is temp count and rc is reader count
	FILE *file;
	/* Allocate array of child PIDs: error handling omitted for brevity */
	rchildPids = malloc(rnum * sizeof(pid_t));
	wchildPids = malloc(wnum * sizeof(pid_t));
	/* Start up children */

	for(i=0; i<rnum; i++) 
	{
   	if((r = fork()) == 0) 
	{

	void* db;
	int rc=0,pid;
	file=fopen("data","r");

	pid=getpid();
	printf("**[reader] process created pid : %d\n",pid);

    	if((shmid=shmget((key_t)2669, 1024, IPC_CREAT|0666)) == -1)
	{
       	perror("shmid failed");
       	exit(1);
   	}
    	if((db=shmat(shmid, (void *)0, 0)) == (void *)-1) 
	{
	perror("shmat failed");
       	exit(1);
    	}

	sem_wait(&mutex);

	readcount++;
	if(readcount==1)
	 sem_wait(&wrt);

	sem_post(&mutex);
	sleep(1);
	while(!feof(file)) // loop until file is empty
	{
	 fscanf(file,"%s\n",(char*)db);//read the file and save into the buffer
	 rc++;
	}sleep(t1);

	sem_wait(&mutex);
	readcount--;
	if(readcount==0)
	 sem_post(&wrt);

	sem_post(&mutex);
	sleep(t1);
	printf("reader_pid : %d has finishied reading %dpicese of data from the data_buffer\n\n",pid,rc);
   	if(shmdt(db) == -1) 
	{
       	perror("shmdt failed");
     	exit(1);
    	}

	fclose(file); // close file for reader


     	exit(0);
   	}
   	else 
	{
     	 rchildPids[i] = r;
  	}
	}
//------------------------------------------------------------------------------------------//
for(j=0; j<wnum; j++) 
	{
   	if((w = fork()) == 0) 
	{
	
	void *db;
	int wc=0,pid,readcount=0;
	pid=getpid();// process id
	printf("**[writer] process created pid : %d\n",pid);
	
    	
	if((shmid=shmget((key_t)2669, 1024, IPC_CREAT|0666)) == -1)
	{
       	perror("shmid failed");
       	exit(1);
   	}
    	if((db=shmat(shmid, (void *)0, 0)) == (void *)-1) 
	{
	perror("shmat failed");
       	exit(1);
   	}
	
	while(!feof(file1))//loop until file is empty
	{
	sem_wait(&wrt);

	fscanf(file1,"%s\n",(char*)db);//read the file1 and write into data buffer
	sleep(t2);
	wc++;	
	
	sem_post(&wrt);
	
	}

	printf("writer_pid : %d has finished writing %d pieces of data to the data_buffer\n\n",pid,wc);

   	if(shmdt(db) == -1) 
	{
       	perror("shmdt failed");
       	exit(1);
    	}
	exit(1);
	}
   	else 
	{
     	 wchildPids[i] = w;
  	}

//----------------- Wait for reader children to exit-------------------------------------------//
	do
	 {
   	 rwaiting = 0;
    	  for (i = 0; i < rnum; i++) 
	  {
       	   if (rchildPids[i] > 0) 
	   {
            if (waitpid(rchildPids[i], NULL, WNOHANG) != 0) 
	    {
             rchildPids[i] = 0;
            }
            else 
	    {
             rwaiting = 1;
	    }
           }
       	   sleep(0);
          }
	 } while (rwaiting);

	free(rchildPids);
//----------------- Wait for writer children to exit-------------------------------------------//
	do
	 {
   	 wwaiting = 0;
    	  for (i = 0; i < wnum; i++) 
	  {
       	   if (wchildPids[i] > 0) 
	   {
            if (waitpid(wchildPids[i], NULL, WNOHANG) != 0) 
	    {
             wchildPids[i] = 0;
            }
            else 
	    {
             wwaiting = 1;
	    }
           }
       	   sleep(0);
          }
	 } while (wwaiting);

	free(wchildPids);
}
}


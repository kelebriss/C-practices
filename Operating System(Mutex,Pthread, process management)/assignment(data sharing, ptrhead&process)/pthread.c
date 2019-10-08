//////////////////////////header/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>       
#include <pthread.h>      
#include <semaphore.h>    
///////////////////////////////////////////////////////////////////
pthread_mutex_t mutex;       // mutex 
pthread_cond_t wrt;	     // pthread condtion

/////////////////////global variable//////////////////////////////////
int db_var= 5; //data buffer size
int readcount;//this is using as flag in this program
int db[5]; // data bufer for testing
FILE *file,*file1;
//////////////////////////////////////////////////////////////////
int initialize(); 
void * Writer(void *arg);
void * Reader(void *arg);
////////////////////////////메인/////////////////////////////////////////////
int main(int argc, char *argv[])
{
	int rdrn,wrtn,t1,t2;//number of reder and writer and sleep time 1 and 2
	int i,j;
	rdrn = atoi(argv[1]);
	wrtn = atoi(argv[2]);
	t1 = atoi(argv[3]);
	t2 = atoi(argv[4]);
	pthread_t writer_thread[wrtn],reader_thread[rdrn];//writer와reader의 thread 선언
	
	initialize();//recall the initialize the mutex,condition and readcount
	file1 = fopen("data","r"); // open for writer
	for(i=0; i<wrtn; i++)
	{
        pthread_create(&writer_thread[i], NULL, &Writer, NULL);//writer thread create
	}
	for(j=0; j<rdrn; j++) //
	{
        pthread_create(&reader_thread[j], NULL, &Reader, NULL);//reader thread create
	}
	for(i=0; i<wrtn; i++)
	{
	pthread_join(writer_thread[i],NULL);//writer thread join
	}
	for(j=0; j<rdrn; j++) //
	{
	pthread_join(reader_thread[j],NULL);//reader thread join
	}
       	pthread_cond_destroy(&wrt);// delete initialize wrt condition
        pthread_mutex_destroy(&mutex);// delete initialize mutex
	fclose(file1); //close for writer
	return 0;
}
int initialize()
{
 pthread_mutex_init(&mutex,NULL);//mutex initialize
 pthread_cond_init(&wrt,NULL);//condition initialize
 readcount=0;//count initialize
}
void * Writer(void * arg)
{	
	int pid,tid,tc=0,wc=0; // tc is temp count wc is writer counter
	tid=pthread_self();//thread id
	pid=getpid();// process id
	printf("**[writer] pthread created pid : %d tid: %d\n",pid,tid);

	pthread_mutex_lock(&mutex);
	while(!feof(file1))//loop until file is empty
	{
	while(readcount=0)//condition 
	{		
	pthread_cond_wait(&wrt,&mutex); //writer wait for signal and release the mutex lock
	}

	readcount=1;
	fscanf(file1,"%d\n",&db[tc]);//read the file1 and write into data buffer
	tc++;
	wc++;
	if(tc == db_var)//if temp count is same as buffer then reset the value.
	{
	tc=0;
	pthread_cond_signal(&wrt); // release the signal for wait condition
	if(!feof(file1)) // exit condtion for last writer thread becasue if there has no if condition then condition wait make function keep waiting.
	pthread_cond_wait(&wrt,&mutex);
	readcount=0;	
	}	
	sleep(1);
	}
	pthread_mutex_unlock(&mutex);//release the mutex
	printf("writer_pid : %d tid : %d has finished writing %d pieces of data to the data_buffer\n\n",pid,tid,wc);
	pthread_cond_signal(&wrt);//release the last writer condition wait

	return NULL;
}

void * Reader(void * arg)  
{
	int pid,tid,tc=0,rc=0; // tc is temp count and rc is reader count
	tid=pthread_self();//thread id
	pid = getpid();//process id
	printf("**[reader] pthread created pid : %d tid : %d\n",pid,tid);

	pthread_mutex_lock(&mutex);//acquire the mutex
	readcount=1;
	file = fopen("data","r"); // open file for reader
	while(!feof(file)) // loop until file is empty
	{
	 fscanf(file,"%d\n",&db[tc]);//read the file and save into the buffer
	 tc++;
	 if(tc == db_var) //if temp counter is same as buffer then add to reader counter and reset
	 {
	  rc+=tc;
	  tc=0;
	  sleep(1);
	 }
	}
	fclose(file); // close file for reader

	pthread_cond_signal(&wrt);//release the signal for wait condition
	pthread_mutex_unlock(&mutex);//release the mutex
	printf("reader_pid : %d tid : %d has finishied reading %dpicese of data from the data_buffer\n\n",pid,tid,rc);

	return NULL;
}



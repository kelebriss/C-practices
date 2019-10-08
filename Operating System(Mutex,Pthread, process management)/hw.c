//////////////////////////헤더파일/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>       
#include <pthread.h>      
#include <semaphore.h>    
///////////////////////////////////////////////////////////////////
	   // thread의 개수
pthread_mutex_t mutex;       // wrt와mutex의 semaphore선언
pthread_cond_t wrt,rdr;
///////////////////////////////////////////////////////////////////


/////////////////////전역변수 선언//////////////////////////////////
int readcount;//reader의카운트수
int db[20];
FILE *file;
//////////////////////////////////////////////////////////////////

int initialize();// 함수 초기화 
void * Writer(void *arg);// Writer 함수
void * Reader(void *arg);// Reader 함수
////////////////////////////메인/////////////////////////////////////////////
int main()
{
	int i,j;//변수
	
	pthread_t writer_thread[5],reader_thread[5];//writer와reader의 thread 선언
	
	initialize();//초기화함수 호출  

	for(j=0; j<5; j++) //
	{
        pthread_create(&reader_thread[j], NULL, &Reader, NULL);	
	}

	for(i=0; i<2; i++)
	{
        pthread_create(&writer_thread[i], NULL, &Writer, NULL);//writer thread 생성
	}

	pthread_join(reader_thread[0],NULL);
	pthread_join(writer_thread[0],NULL);
	pthread_join(reader_thread[1],NULL);
	pthread_join(writer_thread[1],NULL);
	pthread_join(reader_thread[2],NULL);
	pthread_join(reader_thread[3],NULL);
	pthread_join(reader_thread[4],NULL);
        pthread_cond_destroy(&wrt);// 초기화된것을 write삭제
        pthread_cond_destroy(&rdr);// 초기화된것을 write삭제
        pthread_mutex_destroy(&mutex);// 초기화된것을 mutex삭제


	return 0;
}


////////////////초기화함수////////////////////
int initialize()
{
 pthread_mutex_init(&mutex,NULL);//mutex 초기화
 pthread_cond_init(&wrt,NULL);
 pthread_cond_init(&rdr,NULL);
 readcount=0;//카운트 초기화
}
/////////////////////////////////////////////
int test=0,condition=0;

/////////////////////////Writer 함수//////////////////////////////////////////////
void * Writer(void * arg)
{	
	int id,temp,wc=0;
	id=pthread_self()%100;
		printf("**[writer] pthread created tid %d\n",id);
		pthread_mutex_lock(&mutex);
		printf("[writer] fisrt wrt mutex tid %d\n",id);
		while(readcount==0)
		{		printf("[writer] inside wrt cond tid %d\n",id);
		pthread_cond_wait(&wrt,&mutex);
		}
		printf("[writer] doing wrt tid %d\n",id);
		//////////////////////////////////////////////////////
	 	temp = db[wc];
		fprintf(file,"%d\n",temp);
		wc++;
		readcount--;
		///////////////////////////////////////////////////////////////////////////////출력
		printf("*[writer] finish wrt mutex tid %d\n\n",id);		
		pthread_cond_signal(&wrt);
		pthread_mutex_unlock(&mutex);
		printf("writer-%d has finished writing %d pieces of data to the data_buffer\n",getpid(),wc);

	return NULL;
}

void * Reader(void * arg)  
{
	int id,size,rc,tc=0;
	id=pthread_self()%100;
		printf("**[reader] pthread created tid %d\n",id);
		pthread_mutex_lock(&mutex);
		printf("[reader] get in rdr mutex tid %d\n",id);

		pthread_cond_wait(&rdr,&mutex);//writer mutex 다시실행
		/////////////////////////////////////////////////////////////////
		readcount++;
		while(!feof(file))
		{
		 fscanf(file,"%d\n",db[tc]);
		 if(size == tc)
		 {
		  rc+=tc;
		  tc=0;
		 }
		}
		
		/////////////////////////////////////////////////////////////////
		pthread_cond_signal(&wrt);
		printf("*[reader] finish rdr mutex tid %d \n\n",id);
		pthread_mutex_unlock(&mutex);
		printf("reader-%d has finishied reading %dpicese of data from the data_buffer\n",getpid(),rc);
	return NULL;
}

//////////////////////////헤더파일/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>       
#include <pthread.h>      
#include <semaphore.h>    
///////////////////////////////////////////////////////////////////

#define NUMBER 3	   // thread의 개수
sem_t wrt,mutex;       // wrt와mutex의 semaphore선언
///////////////////////////////////////////////////////////////////


/////////////////////전역변수 선언//////////////////////////////////
int rand_value;//rand값을 받기위한변수  
int readcount;//reader의카운트수
//////////////////////////////////////////////////////////////////

int initialize();// 함수 초기화 
void * Writer(void *arg);// Writer 함수
void * Reader(void *arg);// Reader 함수
////////////////////////////메인/////////////////////////////////////////////
int main()
{
	pthread_t   writer_thread, reader_thread[NUMBER];//writer와reader의 thread 선언
	int i;//변수
	int status=0;
	initialize();//초기화함수 호출  
    pthread_create(&writer_thread, NULL, &Writer, &status);//writer thread 생성
	
	//////////////// reader의  thread 생성함//////////////////////////////////
	for(i=1; i<NUMBER; i++) //
	{
        pthread_create(&reader_thread[i], NULL, &Reader, (void*)&i);	  
		sleep(3); //3초간 대기
	}
	/////////////////////////////////////////////////////////////////////////
	
	pthread_join(writer_thread, NULL);// writer가끝날때까지 대기
    sem_destroy(&wrt);// 초기화된것을 write삭제
    sem_destroy(&mutex);// 초기화된것을 mutex삭제

	return 0;
}


////////////////초기화함수////////////////////
int initialize()
{
 sem_init(&wrt,0,1);//wrt 초기화
 sem_init(&mutex,0,1);//mutex 초기화
 return 0;
}
/////////////////////////////////////////////


/////////////////////////Writer 함수//////////////////////////////////////////////
void * Writer(void * arg)
{
	do
	{			
        srand((unsigned)time(NULL));//rand초기화 
		sem_wait(&wrt);//reader끝날때까지 대기
		rand_value =(int)random()%100;//랜덤한값생성
		printf("Writer THREAD0 put %d\n",rand_value);//출력
        sleep(1);//1초대기
		sem_post(&wrt);//다시실행함
		sleep(3);//3초대기
	}while(1);

}
/////////////////////////////////////////////////////////////////////////////////


///////////////////////////Reader 함수/////////////////////////////////////////
void * Reader(void * arg)  
{
	int id;  //thread의 id	id=*(int *)arg;
	readcount=0;//카운트 초기화

    do
	{
		sem_wait(&mutex);//대기(상호배제)
		readcount++;//카운트증가    
		if ( readcount==1 )//1일때
        sem_wait(&wrt);//실행될때까지대기
		sem_post(&mutex);//다시실행

		printf("Reader THREAD%d first get %d\n", id, rand_value);//출력
		sleep(1);//1초대기
		printf("Reader THREAD%d second get %d\n", id, rand_value);//출력

		sem_wait(&mutex);//실행될때까지대기
		readcount--;//reader실행완료후 카운트감소
		if ( readcount==0 )//카운트가 0일때
		sem_post(&wrt);//writer다시실행
		sem_post(&mutex);//다시실행   
		sleep(3);//3초간대기
	}while(1);

}

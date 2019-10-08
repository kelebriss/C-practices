////////////////������� ����///////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
#define THREADNUM 3 //process�� ��
#define MAXITEM 100 //�ִ�����ۼ�
/////////////////////////////////////////////////////

//////////////////����ü ����///////////////////////
struct data
{
	int buff[MAXITEM];//����
	int cnt_item;//���ۿ� ������ �������� �Ѱ���
	int cnt_slot;//���ۿ��� �о���� �����ۼ�
}Data;
//////////////////////////////////////////////////////

pthread_mutex_t mutex;//mutex����

void *Producer(void *data);//Producer����
void *Consumer(void *data);//Consumer����

int main()
{
	pthread_t producer[THREADNUM],consumer;// producer��consumer�� thread����
    
	////��������/////////////
	int i=0;//
	int a =1;
	int count[THREADNUM];
	//////////////////////////
    
	//////////////�ʱ�ȭ///////
	Data.cnt_item = 0;		
	Data.cnt_slot =0;
	///////////////////////////
    
	///////////////////producer thread����///////////////////////////
	for(i=0;i<THREADNUM;i++)
    {
		pthread_create(&producer[i],NULL,Producer,(void *)&i);
	}
	////////////////////////////////////////////////////////////////

	/////////////////////////consumer thread����///////////////////
	pthread_create(&consumer,NULL,Consumer,(void *)&a);
	
	///////producer�� consumer join///////////////////////////
	for(i=0;i<THREADNUM;i++)// ���������� main���
	{
	  pthread_join(producer[i],NULL);//producer ������
	  pthread_join(consumer,NULL);//consumer ������
	}
	////////////////////////////////////////////////////////
	return 0;
}

void *Producer(void *data)//Producer�Լ�
{
	/////��������//////
	int threadNum;
	int i;
    ///////////////////

	threadNum=*(int *)data;//��������̵�

    while(Data.cnt_item<MAXITEM)//�ִ�ũ�� ������
	{
		if(Data.cnt_slot < 0)   //overflow����
		{
			pthread_mutex_unlock(&mutex);//unlock
			sleep(1);//1�ʴ��
		}
		pthread_mutex_lock(&mutex);//mutex lock��û
        Data.buff[Data.cnt_item]=Data.cnt_item*Data.cnt_item;//�����ǵ�����
        printf("Producer THREAD%d put %d on the buffer[%d]      cnt_item %d\n",//���
		threadNum,Data.buff [Data.cnt_item],Data.cnt_item,Data.cnt_item+1);
		Data.cnt_item++;//���������Ƿ� ����
		pthread_mutex_unlock(&mutex);//unlock ��û
        sleep(2);//2�ʴ��
	}	
}
void *Consumer(void *data)//consumer�Լ� 
{
	while(Data.cnt_slot<MAXITEM)//�ִ�ũ��������
	{
      pthread_mutex_lock(&mutex);//unlock��û
      Data.buff[Data.cnt_slot]=Data.cnt_slot*Data.cnt_slot;//�����ǵ�����
      printf("Consumer THREAD3 put %d on the buffer[%d]      cnt_item %d\n", Data.buff [Data.cnt_slot],Data.cnt_slot,Data.cnt_slot+1);//���
      Data.buff[Data.cnt_slot]=0;//�Һ��Ͽ����Ƿ� �ʱ�ȭ		
      Data.cnt_slot++;//����������
	  pthread_mutex_unlock(&mutex);//unlock
      sleep(1);//1�ʴ��
	}
}


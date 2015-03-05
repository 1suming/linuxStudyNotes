/**
semaphoreSolution.cpp

*/
#include <iostream> 
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>

using namespace std;

#define MAX_SIZE 5

vector<int> buffer; //�������ݻ�����
pthread_mutex_t mutex;          //���������ʻ�����
sem_t full; //���������������ź���
sem_t empty; //���������������ź���

void *producer(void *arg)
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		sem_wait(&empty);//���������п��пռ䣬�����ݣ���������
		pthread_mutex_lock(&mutex);
		
		buffer.push_back(i);
		cout<<"Producer:"<<i<<endl;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		sleep(0.02);
	}
}

void *consumer(void *arg)
{
	int nMsgCount=0;
	while(nMsgCount<MAX_SIZE)
	{
		sem_wait(&full);/*�����������ݣ�ȡ���ݣ���������*/
		pthread_mutex_lock(&mutex);
		
		vector<int>::iterator iter=buffer.begin();
		int value=*iter;
		buffer.erase(iter);
		cout<<"consumer:"<<value<<endl;
		pthread_mutex_unlock(&mutex);
		
		sem_post(&empty);
		nMsgCount++;
		sleep(1);
	}
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full,0,0);
	sem_init(&empty,0,MAX_SIZE);
	
	pthread_t pro,con;
	pthread_create(&pro,NULL,producer,NULL);
	pthread_create(&con,NULL,consumer,NULL);
	
	 
	sem_destroy(&full);
	sem_destroy(&empty);
	
	pthread_join(pro,NULL);
	pthread_join(con,NULL);
}



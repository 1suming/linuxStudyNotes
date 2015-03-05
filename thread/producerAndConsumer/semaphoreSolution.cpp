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

vector<int> buffer; //共享数据缓冲区
pthread_mutex_t mutex;          //缓冲区访问互斥量
sem_t full; //缓冲区有有数据信号量
sem_t empty; //缓冲区空闲数据信号量

void *producer(void *arg)
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		sem_wait(&empty);//缓冲区若有空闲空间，放数据，否则阻塞
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
		sem_wait(&full);/*缓冲区有数据，取数据，否则阻塞*/
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



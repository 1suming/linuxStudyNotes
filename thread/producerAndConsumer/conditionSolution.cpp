#include<stdio.h>
#include<pthread.h>

/**
 conditionSolution.cpp 
 
 */
#define MAX 10 /*需要生产的数量*/

pthread_mutex_t mutex;
pthread_cond_t condc,condp;
int buffer=0; /*生产者、消费者使用的缓冲区*/

void *producer(void *ptr)
{
	int i;
	for(i=1;i<=MAX;i++)
	{
		pthread_mutex_lock(&mutex);/*互斥使用缓冲区*/
		while(buffer!=0)
			pthread_cond_wait(&condp,&mutex);
		printf("Producer produce item %d\n",i);
		buffer=i;
		pthread_cond_signal(&condc);/*唤醒消费者 */
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

void *consumer(void *ptr)
{
	for(int i=1;i<=MAX;i++)
	{
		pthread_mutex_lock(&mutex);
		while(buffer==0)
			pthread_cond_wait(&condc,&mutex);
		printf("Consuerm consume item %d\n",i);
		buffer=0;/*从缓冲区取出数据*/
		pthread_cond_signal(&condp);/*唤醒生产者*/
		pthread_mutex_unlock(&mutex);
	}
}
int main()
{
	pthread_t pro,con;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&condc,NULL);
	pthread_cond_init(&condp,NULL);
	
	pthread_create(&pro,NULL,producer,NULL);
	pthread_create(&con,NULL,consumer,NULL);
	
	pthread_join(pro,0);
	pthread_join(con,0);
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&mutex);
}
	
		
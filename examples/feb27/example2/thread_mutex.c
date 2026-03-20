//This example demonstrates using mutexes.
//It involves three threads.
//It was written by Andy Mitofsky
#include "../../../se233.h"
//declare functions
static void * threadOne(void * arg);
static void * threadTwo(void * arg);

int x=0; //Global variable that threads want to access

int main()
{

	int ret=0;
	int j=0;
	char inValue[50];
	pthread_t threadA;
	pthread_t threadB;
	pthread_mutex_t myMutex=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t *mptr;
	mptr=&myMutex;


	//start the threads
	ret=pthread_create(&threadA, NULL, threadOne, mptr);
	if(ret!=0)
	{
		errno=ret;
		perror("Thread one error");
	}
	ret=0;
	ret=pthread_create(&threadB, NULL, threadTwo, mptr);
	if(ret!=0)
	{
		errno=ret;
		perror("Thread two error");
	}

	printf("Main thread says hello \n");


	//stop the threads cleanly
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);
	
	return 0;
}

//define functions


void * threadOne(void * arg)
{

	pthread_mutex_lock(arg);


	printf("Thread One is best \n");
	int j=0;
	for (j=0;j<100;j++)
	{	
		printf("Thread one says x=%d \n", x);
		x++;
	}
	pthread_mutex_unlock(arg);

	return 0;

}

void * threadTwo(void * arg)
{
	pthread_mutex_lock(arg);
	printf("Thread Two is great \n");
	int k=0;
	for(k=0;k<100;k++)
	{
		printf("Thread two says %d \n", x);
		x++;
	}
	pthread_mutex_unlock(arg);


	return 0;
}
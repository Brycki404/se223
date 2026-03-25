//This example demonstrates starting a new thread.
//It was written by Andy Mitofsky
#include "../../../se233.h"
//declare a new function
static void * thread_func(void * arg);

int main()
{

	int ret=0;
	int j=0;
	pthread_t threader;

	ret = pthread_create(&threader, NULL, thread_func, NULL);
	if(ret!=0)
	{
		errno=ret;
		perror("Thread create error");
	}
	printf("Main thread says hi \n");
	pthread_join(threader, NULL);

	return 0;
}

//define the function that the new thread follows
static void * thread_func(void * arg)
{
	printf("New thread says hi \n");
	return 0;
}
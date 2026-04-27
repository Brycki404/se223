//This example forks a child and both access memory repeatedly.
//It was written by Andy Mitofsky
#include "../se233.h"

int main()
{
	int i, r;
	pid_t mypid;
	pid_t kidpid;
	pid_t wpid;
	int x, y;
	x=0;

	//Fork the child process
	kidpid=fork();
	if(kidpid<0)
	{
		perror("Fork error");
	}


	//Both update a variable.
	//Both the parent and the child have their own copies of variables.

	mypid=getpid();
	if(mypid<0)
	{
		perror("getpid error");
	}
	for(i=0; i<2000; i++)
	{
		printf("%d says %d \n", mypid, x);
		x++;
	}

	if(kidpid!=0)
	{
		//We're in the parent. The parent waits on the child
		wpid=wait(&y);
	}

	return 0;
}

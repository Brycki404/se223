//This program demonstrates fork and execl.
//It was written by Andy Mitofsky
#include "../se233.h"

int main()
{

	int i, y, r;
	pid_t mypid;
	pid_t kidpid;
	pid_t wpid;
	pid_t myppid;

	//Let's display our pid and ppid
	if((mypid=getpid())<0)
	{
		perror("Getpid error");
	}
	printf("My pid is %d \n", mypid);
	if((myppid=getppid())<0)
	{
		perror("Getppid error");
	}
	printf("My ppid is %d \n", myppid);

	//Fork the child
	kidpid=fork();
	if(kidpid<0)
	{
		perror("Fork error");
	}
	if(kidpid==0)
	{
		//We're in the child
		printf("Child says hi \n");
		r=execl("hello.o", "hello.o", NULL);
		if(r==-1)
		{
			perror("Execl error");
		}
	}
	else 
	{
		//We're in the parent
		printf("The parent says hello \n");
		//Parent should wait on the child
		wpid=wait(&y);
		if(wpid<0)
		{
			perror("Wait error");
		}

	}


	return 0;
}

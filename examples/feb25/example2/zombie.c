//This example demonstrates a zombie process, a process that has
//ended but hasn't yet been waited on by its parent.
//It was written  by Andy Mitofsky closely following the book by Stevens p. 283

#include "../se233.h"
int main()
{
	pid_t mypid;
	pid_t kidpid=0;
	pid_t wpid=0;
	int y;

	void child_fn(void *arg)
	{
		//We're in the child
		printf("Kid says hi \n");
		return 0;
	}

	if((mypid=getpid())==-1)
	{
		perror("getpid error");
	}
	//fork the child
	kidpid=xp_fork(child_fn);
	if(kidpid==-1)
	{
		perror("fork error");
	}
	if(kidpid>0)
	{
		//We're in the parent
		printf("Parent says hello \n");
		sleep(1);
		if(system("ps ax | grep zombieEx")==-1)
		{
			perror("system error");
		}
		sleep(1);
		//Now we'll wait on the child
		wpid=xp_waitpid(kidpid, &y, 0);
		printf("We have now properly waited on the child process \n");
		if(system("ps ax | grep zombieEx")==-1)
		{
			perror("system error");
		}
		
	}

	return 0;
}
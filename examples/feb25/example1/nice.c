//This example demonstrates the nice systems call
//It was written by Andy Mitofsky
#include "../../../se233.h"
int main()
{
	pid_t mypid;
	int nv=0;
	mypid=xp_getpid();
	if(mypid<0)
	{
		perror("mypid error");
	}
	errno=0;
	xp_nice(0);
	if((nv==-1)&&(errno!=0))
	{
		perror("nice error");
	}
	printf("Nice value of process %d is %d \n", mypid, nv);
	errno=0;
	nv=xp_nice(-3);
	if((nv==-1)&&(errno!=0))
	{
		perror("nice error");
	}
	printf("Nice value of process %d is now %d \n", mypid, nv);


	return 0;
}
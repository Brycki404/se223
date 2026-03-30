//This example demonstrates popen. This process writes to the pipe. We'll use less to read from it. It was written by Andy Mitofsky.
//
#include "../se233.h"
int main(int argc, char * argv[])
{
	FILE * fp;
	if(argc<2)
	{
		printf("You need to include a message");
		return -1;
	}
	
	if((fp=popen("less", "w"))==NULL)
	{
		printf("popen error");
	}
	fputs(argv[1], fp);
	fputs(argv[2], fp);
	if(pclose(fp)==-1)
	{
		printf("Pipe close error");
	}
	return 0;
}
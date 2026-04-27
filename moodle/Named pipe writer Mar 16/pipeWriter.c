//This program sets up a named pipe and writes to it.
//It was written by Andy Mitofsky
#include "../se233.h"
int main()
{
	int fd;
	char *myfifo="./myfifo";
	if(mkfifo(myfifo, 0666)==-1)
	{
		perror("mkfifo error");
	}
	fd=open(myfifo, O_WRONLY);
	if(fd==-1)
	{
		perror("Open error");
	}
	if(write(fd, "strawberry", sizeof("strawberry"))==-1)
	{
		perror("Write error");
	}
	if(close(fd)==-1)
	{
		perror("close error");
	}
	if(unlink(myfifo)==-1)
	{
		perror("unlink error");
	}

	return 0;
}

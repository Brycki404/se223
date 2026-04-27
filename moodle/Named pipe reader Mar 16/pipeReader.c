//This example reads from a named pipe.
//It was written by Andy Mitofsky
#include "../se233.h"
int main()
{
	int fd;
       	int buf_sz=100;
	char *myfifo ="./myfifo";
 	char buf[buf_sz];	

	fd=open(myfifo, O_RDONLY);
	if(fd==-1)
	{
		perror("Open error");
	}
	if(read(fd, buf, buf_sz)==-1)
	{
		perror("Read error");
	}
	printf("Received %s \n", buf);
	if(close(fd)==-1)
	{
		perror("close error");
	}

	return 0;
}

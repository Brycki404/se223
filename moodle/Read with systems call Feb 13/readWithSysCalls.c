//This example uses systems calls to read.
//It does not include error checking.
#include "../se233.h"
int main()
{
	int fd;
	ssize_t bufsize =4096;
	char buf[bufsize];

	fd=open("colors.txt", O_RDONLY);
	read(fd, buf, bufsize);
	printf(buf);
	if(close(fd)<0)
	{
		perror("Close error");
	}
	
	return 0;
}

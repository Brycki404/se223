// This example demonstrates writing with systems calls
// It was written by Andy Mitofsky

#include "../../se233.h"

int main()
{
    int fd;
    char buf[100];
    fd = open("examples/feb16/veggies", O_WRONLY | O_CREAT);
    if (fd<0)
    {
        perror("Open error");
    }
    strcpy(buf, "carrot");
    if (write(fd, buf, strlen(buf)) < 0)
    {
        perror("Write error");
    }
    if (close(fd) < 0)
    {
        perror("Close error file");
    }
    return 0;   
}
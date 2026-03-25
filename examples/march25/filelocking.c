//This example demonstrates locking a file.
//This processs forks a child. The parent locks the file and writes. The child tries to access it but has to wait.
//It was written by Andy Mitofsky.
#include "../../se233.h"

void xp_fork_child(void *arg)
{
    printf("Child says hi \n");
    const char *kidbuf = "pepper";
    sleep(2);

    printf("Child is trying to get lock\n");

    int fd = open("veggies.txt", O_CREAT | O_APPEND | O_RDWR);
    if (fd == -1)
    {
        perror("open error");
    }

    if (lockf(fd, F_LOCK, 0) == 0) {
        if (write(fd, kidbuf, strlen(kidbuf)) == -1)
        {
            perror("write error");
        }
        printf("Child wrote\n");
    }
    else
    {
        perror("child can't get lock");
    }

    if (lockf(fd, F_ULOCK, 0) == -1) {
        perror("lockf error");
    }

    if (close(fd) == -1) {
        perror("close error");
    }

    printf("Child is done\n");

    #ifdef _WIN32
        // Threads must exit explicitly
        ExitThread(0);
    #endif
}

int main()
{
    int fd;
    pid_t mypid;
    pid_t kidpid;
    pid_t wpid;
    int y;

    kidpid = xp_fork();
    if (kidpid == -1)
    {
        perror("Fork error");
    }
    else if (kidpid == 0)
    {
        #ifndef _WIN32
            // Linux child runs here
            xp_fork_child(NULL);
         #endif
    }
    else
    {
        //We're in the parent
        printf("Parent says hi\n");
        const char * parentbuf="carrot \n";
        fd=open("veggies.txt", O_CREAT|O_APPEND|O_RDWR);
        if (fd==-1)
        {
            perror("open error");
        }
        if (chmod("veggies.txt", (S_IRWXU|S_IRWXG|S_IRWXO))==-1)
        {
            perror("fchmod error");
        }
        //lock the file
        if (lockf(fd, F_LOCK, 0)==-1)
        {
            perror("lockf error");
        }
        //write to the file
        if (write(fd, parentbuf, strlen(parentbuf))==-1)
        {
            perror("write error");
        }
        printf("Parent wrote and is holding the lock for a bit \n");
        sleep(3);
        //parent release the lock
        if (lockf(fd, F_ULOCK, 0)==-1)
        {
            perror("lockf error");
        }
         printf("Parent is done \n");
         if (close(fd)==-1)
         {
             perror("close error");
         }
    }
}
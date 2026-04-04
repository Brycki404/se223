// This example sets up a socket between a parent and child process.
// The child is the client and writes. The parent is the server and reads.
// This was written by Andy Mitofsky.
// It closely follows the book by Rochkind p. 522.

// For Linux

#include "../se223.h"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCKETNAME "MySocket"

int main()
{
    struct sockaddr sa;
    (void) unlink (SOCKETNAME);
    strcpy(sa.sa_data, SOCKETNAME);
    sa.sa_family = AF_UNIX;

    //fork a child
    if(fork() == 0)
    {
        //We're in the child process. The child is the
        //client that writes.
        //The child uses socket and connect

        int fd_skt;
        char buf[100];
        if((fd_skt = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        {
            perror("Socket error in child");
        }
        while (connect(fd_skt, (const struct sockaddr *) &sa, sizeof(sa)) == -1)
        {
            if (errno == ENOENT)
            {
                sleep(1);
                continue;
            }
        }
        //The child can write or read
        if (write(fd_skt, "Turnip", sizeof("Turnip")) == -1)
        {
            perror("Write error in child");
        }
        if (read(fd_skt, buf, sizeof(buf)) == -1)
        {
            perror("Read error in child");
        }
        printf("Child got %s\n", buf);
        //child closes socket
        if (close(fd_skt) == -1)
        {
            perror("Close error in child");
        }
        exit(0);
    } else
    {
        //We're in the parent process.
        //The parent is the server and reads
        //The parent uses socket, bind, listen, and accept.
        int fd_skt;
        int fd_client;
        char buf[100];

        fd_skt = socket(AF_UNIX, SOCK_STREAM, 0);
        if (bind(fd_skt, (const struct sockaddr *) &sa, sizeof(sa)) == -1)
        {
            perror("Bind error in parent");
        }
        listen(fd_skt, SOMAXCONN);
        fd_client = accept(fd_skt, NULL, 0);
        if(fd_client == -1)
        {
            perror("Accept error in parent");
        }
        // Here the parent is all set up and can write and read
        if (read(fd_client, buf, sizeof(buf)) == -1)
        {
            perror("Read error in parent");
        }
        if (write(fd_client, "paprika", sizeof("paprika"))==-1)
        {
            perror("Write error in parent");
        }
        if (close(fd_skt) == -1)
        {
            perror("Close error in parent");
        }
        if (close(fd_client) == -1)
        {
            perror("Close error in parent");
        }

        wait(NULL);
    }

    return 0;
}
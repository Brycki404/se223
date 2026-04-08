// Written by Dustin Knecht for the exam on 4.8.2026 in SE233
// To be ran on Linux

//Header  file for SE233 updated 1-22-25

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
// LINUX ONLY:
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <pthread.h>

//declare a new function
static void * five_handler(void * arg);
static void eight_handler(int sig);

int main(void)
{
    if (signal(SIGUSR1, eight_handler) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }

    char name[256];
    printf("Enter your name: ");
    if (scanf("%255s", name) != 1)
    {
        fprintf(stderr, "Invalid input. Enter a string that's less than 256 characters long.\n");
        return 1;
    }

   int n;
    printf("Enter a number: ");
    if (scanf("%d", &n) != 1)
    {
        fprintf(stderr, "Invalid input. Enter a valid integer.\n");
        return 1;
    }

    if (n == 8)
    {
        if (kill(getpid(), SIGUSR1) == -1)
        {
            perror("kill");
            return 1;
        }
    }
    else if (n == 5)
    {
        int ret = 0;
        pthread_t threadId;

        ret = pthread_create(&threadId, NULL, five_handler, NULL);
        if (ret != 0)
        {
            errno = ret;
            perror("Thread create error");
        }
        printf("Main thread says hi \n");
        pthread_join(threadId, NULL);
    }
    else if (n == 0)
    {
        
    }
    else
    {
        printf("Number is not mapped to any special case.\nTry entering 0, 5, or 8.\n");
    }
    
    return 0;
}

static void eight_handler(int sig)
{
    (void)sig;
    printf("I'm quitting now.\n");
    exit(0);
}

static void * five_handler(void * arg)
{
    for (int i = 1; i <= 5 ; i++)
    {
        if (i > 1)
        {
            sleep(1); // seconds
        }
        printf("GREAT");
    }
    return 0;
}
// This program demonstrates fork and execl.
// Linux only!
// It was written by Andy Mitofsky
#include "../../../se233.h"


// Bash Fork Bomb:
// :(){ :|:& };:

// Explanation of the Script

// : () defines a function named :.

// { :|: & } runs the function : and pipes its output to another instance of the same function, running in the background.

// ; separates the function definition from its execution.

// : calls the function for the first time.

// This script creates a function that calls itself twice every time it is executed, leading to an exponential growth in the number of processes until the system's resources are exhausted.


int main()
{
    int i, y, r;
    pid_t mypid;
    pid_t kidpid;
    pid_t wpid;
    pid_t myppid;

    // Let's display our pid and ppid
    mypid = xp_getpid();
    if (mypid < 0)
    {
        perror("Getpid error");
    }
    printf("My pid is %d \n", mypid);
    myppid = xp_getppid();
    if (myppid < 0)
    {
        perror("Getppid error");
    }
    printf("My ppid is %d \n", myppid);

    void child_fn(void *arg)
    {
        // We're in the child
        printf("Child says hi \n");
        r = xp_execl("hello.o", "hello.o", NULL);
        if (r == -1)
        {
            perror("Execl error");
        }
    }

    // Fork the child
    kidpid = xp_fork(child_fn, NULL);
    if (kidpid < 0)
    {
        perror("Fork error");
    }
    if (kidpid > 0)
    {
        // We're in the parent
        printf("The parent says hello \n");
        // Parent should wait on the child
        wpid = xp_waitpid(kidpid, &y, 0);
        if (wpid < 0)
        {
            perror("Wait error");
        }
    }

    return 0;
}
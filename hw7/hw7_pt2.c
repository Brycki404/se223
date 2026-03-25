#include "../se233.h"

void c1_fn(void *arg)
{
    // First child
    printf("I'm child 1, and I'm counting from 1 to 10.\n");
    fflush(stdout);

    for (int i = 1; i <= 10; i++) {
        printf("%d\n", i);
        fflush(stdout);
        if (i < 10)
        {
            sleep(2);
        }
    }
    
    xp_exit(EXIT_SUCCESS);
}

void c2_fn(void *arg)
{
    // Second child
    // Lower priority (increase nice value)
    if (xp_nice(10) == -1 && errno != 0) {
        perror("nice");
        // continue anyway
    }

    printf("I'm child 2.\n");
    fflush(stdout);

    long sum = 0;
    for (int i = 500; i <= 600; i++) {
        sum += i;
    }

    printf("Child 2 sum from 500 to 600 is %ld\n", sum);
    fflush(stdout);

    xp_exit(EXIT_SUCCESS);
}

int main(void) {
    pid_t c1;
    pid_t c2;

    c1 = xp_fork(c1_fn, NULL);
    if (c1 < 0) {
        perror("fork child1");
        xp_exit(EXIT_FAILURE);
    }

    // Parent continues, fork second child
    c2 = xp_fork(c2_fn, NULL);
    if (c2 < 0) {
        perror("fork child2");
        // Still wait on first child
        int status;
        xp_waitpid(c1, &status, 0);
        xp_exit(EXIT_FAILURE);
    }

    // Parent
    printf("Hello from the parent\n");
    fflush(stdout);

    int status;
    if (xp_waitpid(c1, &status, 0) == -1) {
        perror("xp_waitpid child1");
    }
    if (xp_waitpid(c2, &status, 0) == -1) {
        perror("xp_waitpid child2");
    }

    return 0;
}
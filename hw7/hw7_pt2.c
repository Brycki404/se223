//Header  file for SE233 updated 1-22-25

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int main(void) {
    pid_t c1, c2;

    c1 = fork();
    if (c1 < 0) {
        perror("fork child1");
        exit(EXIT_FAILURE);
    }

    if (c1 == 0) {
        // First child
        printf("I'm child 1, and I'm counting from 1 to 10.\n");
        fflush(stdout);

        for (int i = 1; i <= 10; i++) {
            printf("%d\n", i);
            fflush(stdout);
            sleep(2);
        }
        exit(EXIT_SUCCESS);
    }

    // Parent continues, fork second child
    c2 = fork();
    if (c2 < 0) {
        perror("fork child2");
        // Still wait on first child
        int status;
        waitpid(c1, &status, 0);
        exit(EXIT_FAILURE);
    }

    if (c2 == 0) {
        // Second child
        // Lower priority (increase nice value)
        if (nice(10) == -1 && errno != 0) {
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

        exit(EXIT_SUCCESS);
    }

    // Parent
    printf("Hello from the parent\n");
    fflush(stdout);

    int status;
    if (waitpid(c1, &status, 0) == -1) {
        perror("waitpid child1");
    }
    if (waitpid(c2, &status, 0) == -1) {
        perror("waitpid child2");
    }

    return 0;
}
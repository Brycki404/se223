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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Child mode: ./hw7_p1 child
    if (argc >= 2 && strcmp(argv[1], "child") == 0) {
        pid_t pid = getpid();
        printf("Child %d says the magic word is Corn\n", pid);
        fflush(stdout);
        return 0;
    }

    // Parent mode
    const char *filename = argv[1];

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child: replace with same program in "child" mode
        if (execl(argv[0], argv[0], "child", (char *)NULL) == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent: write to file
        FILE *fp = fopen(filename, "w");
        if (!fp) {
            perror("fopen");
            // Still wait for child before exiting
            int status;
            waitpid(pid, &status, 0);
            exit(EXIT_FAILURE);
        }

        pid_t mypid = getpid();
        if (fprintf(fp, "Process %d says the magic word is Maize\n", mypid) < 0) {
            perror("fprintf");
            fclose(fp);
            int status;
            waitpid(pid, &status, 0);
            exit(EXIT_FAILURE);
        }

        if (fclose(fp) == EOF) {
            perror("fclose");
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
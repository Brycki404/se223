#include "../se233.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        xp_exit(EXIT_FAILURE);
    }

    // Child mode: ./hw7_p1 child
    if (argc >= 2 && strcmp(argv[1], "child") == 0) {
        pid_t pid = xp_getpid();   // wrapper for getpid()
        printf("Child %d says the magic word is Corn\n", pid);
        fflush(stdout);
        xp_exit(0);
    }

    // Parent mode
    const char *filename = argv[1];

    // Fork child
    pid_t pid = xp_fork(
        /* child function */
        [](void *arg) {
            char **argv = (char **)arg;
            // exec into child mode
            if (xp_execl(argv[0], argv[0], "child", NULL) == -1) {
                perror("xp_execl");
                xp_exit(EXIT_FAILURE);
            }
        },
        argv   // pass argv to child
    );

    if (pid < 0) {
        perror("xp_fork");
        xp_exit(EXIT_FAILURE);
    }

    // Parent continues
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        int status;
        xp_waitpid(pid, &status, 0);
        xp_exit(EXIT_FAILURE);
    }

    pid_t mypid = xp_getpid();
    if (fprintf(fp, "Process %d says the magic word is Maize\n", mypid) < 0) {
        perror("fprintf");
        fclose(fp);
        int status;
        xp_waitpid(pid, &status, 0);
        xp_exit(EXIT_FAILURE);
    }

    if (fclose(fp) == EOF) {
        perror("fclose");
    }

    int status;
    if (xp_waitpid(pid, &status, 0) == -1) {
        perror("xp_waitpid");
        xp_exit(EXIT_FAILURE);
    }

    return 0;
}
#include "../se233.h"

// Simple haiku (example, cite if you use a real one)
// Source: Example haiku, not from a specific poet.
void *poet_thread(void *arg) {
    (void)arg;

    printf("I'm a poet\n");
    printf("Silent morning light\n");
    printf("Whispers through the waking trees\n");
    printf("Day begins to breathe\n");
    fflush(stdout);

    return NULL;
}

void *worker_thread(void *arg) {
    const char *filename = (const char *)arg;

    printf("I'm a worker\n");
    fflush(stdout);

    // Print Linux version
    int ret = system("uname -r");
    if (ret == -1) {
        perror("system uname -r");
    }

    // Change permissions: rwx for user, group, others (0777)
    if (chmod(filename, 0777) == -1) {
        perror("chmod");
    } else {
        printf("Permissions for '%s' set to 777\n", filename);
    }

    fflush(stdout);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_to_chmod>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *target_file = argv[1];

    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, poet_thread, NULL) != 0) {
        perror("pthread_create poet");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&t2, NULL, worker_thread, (void *)target_file) != 0) {
        perror("pthread_create worker");
        // Try to join t1 before exiting
        pthread_join(t1, NULL);
        exit(EXIT_FAILURE);
    }

    if (pthread_join(t1, NULL) != 0) {
        perror("pthread_join poet");
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("pthread_join worker");
    }

    return 0;
}
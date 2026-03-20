// Dustin Knecht - hw4
#include "../se233.h"

int main(int argc, char *argv[]) {
    // argv[0] is the program name, so we need at least 2 arguments for a name
    if (argc < 2) {
        printf("Usage: %s <name> [options]\n", argv[0]);
        return 1;
    }

    // Person's name is always argv[1]
    char *name = argv[1];

    int seen_v = 0;
    int seen_r = 0;

    // Seed RNG for -r option
    time_t t;
    srand((unsigned) time(&t));

    printf("Welcome %s\n", name);

    // Process options starting from argv[2]
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 && !seen_v) {
            printf("It is so wonderful to see you again on this illustrious computer.\n");
            seen_v = 1;
        }
        else if (strcmp(argv[i], "-r") == 0 && !seen_r) {
            int r = (rand() % 6) + 1;  // random 1–6
            printf("I picked random number %d. Have a", r);

            for (int j = 0; j < r; j++) {
                printf(" great");
            }

            printf(" day.\n");
            seen_r = 1;
        }
        // Ignore all other options
    }

    return 0;
}
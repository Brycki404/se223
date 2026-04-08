#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void huge_handler(int sig) {
    (void)sig;
    printf("That number is huge!\n");
    exit(0);
}

int main(void) {
    if (signal(SIGUSR1, huge_handler) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    int n;
    printf("Enter a number: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (n > 1000) {
        if (kill(getpid(), SIGUSR1) == -1) {
            perror("kill");
            return 1;
        }
    } else {
        printf("Number is not huge.\n");
    }

    return 0;
}
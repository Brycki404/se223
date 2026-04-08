// Written by Dustin Knecht for the exam on 4.8.2026 in SE233

#include "../se233.h"

//declare a new function
static void five_handler();
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

    while (1)
    {
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
           
        }
        else if (n == 0)
        {
            
        }
        else
        {
            printf("Number is not mapped to any special case.\nTry entering 0, 5, or 8.\n");
        }
    }

    return 0;
}

void eight_handler(int sig)
{
    (void)sig;
    printf("I'm quitting now.\n");
    exit(0);
}

void five_handler()
{
    for (int i = 1; i <= 5 ; i++)
    {
        sleep(1); //ms
        printf("GREAT", i);
    }
    exit(0);
}
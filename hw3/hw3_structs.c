// Dustin Knecht - hw3.2
#include "../se233.h"
#include <stdbool.h>

struct movie {
    float runtime;
    char title[50];
    char genre[50];
};

int main()
{
    struct movie a;
    strcpy(a.genre, "Piratey");
    strcpy(a.title, "Pirates");
    a.runtime = 1.5;

    struct movie b;
    strcpy(b.genre, "Spacey");
    strcpy(b.title, "Spaceships");
    b.runtime = 2.0;

    printf("Movie a: %s (%s, %g)\n", a.title, a.genre, a.runtime);
    printf("Movie b: %s (%s, %g)", b.title, b.genre, b.runtime);
    return 0;
}
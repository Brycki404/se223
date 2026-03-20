// Dustin Knecht - hw3.1
#include "../se233.h"
#include <stdbool.h>

int main()
{
    float side1;
    float side2;
    float angle;
    while (1)
    {
        printf("Give me the length of a side of a triangle:");
        scanf("%g", &side1);

        printf("\nNow give me the length of a second adjacent side of that triangle:");
        scanf("%g", &side2);

        printf("\nNow give me the angle between the two adjacent sides:");
        scanf("%g", &angle);

        float side3_squared = pow(side1, 2) + pow(side2, 2) - 2 * side1 * side2 * cos(angle);
        printf("\nSide3 squared: %g", side3_squared);

        float side3 = sqrt(side3_squared);
        printf("\nSide3: %g", side3);

        printf("\nWould you like to run the program again? [y/n]: ");
        char response;
        scanf(" %c", &response);
        //That leading space tells scanf to skip whitespace (including newlines).

        if (response != 'y')
        {
            break;
        }
    }
    printf("\nExiting program...");   
    return 0;
}
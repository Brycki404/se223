// Dustin Knecht - hw3.3
#include "../se233.h"
#include <stdbool.h>

//Define struct
struct mosaic {
    int redTiles;
    int blueTiles;
    int greenTiles;
    int whiteTiles;
};

//Declare functions
int totalTiles(int redTiles, int blueTiles, int greenTiles, int whiteTiles);
int maxTiles(struct mosaic * mosaicptr);
void tripleTiles(struct mosaic * mosaicptr);

//Define main
int main()
{
    struct mosaic a;
    struct mosaic * aptr = &a;
    a.redTiles = 0;
    a.blueTiles = 1;
    a.greenTiles = 2;
    a.whiteTiles = 3;

    int sum = totalTiles(a.redTiles, a.blueTiles, a.greenTiles, a.whiteTiles);

    printf("Sum of mosaic 'a' is: %d", sum);

    int highest = maxTiles(aptr);

    printf("\nHighest Tile Count: %d", highest);

    tripleTiles(aptr);

    printf("\nResulting mosiac 'a' is: {\nredTiles=%d,\nblueTiles=%d,\ngreenTiles=%d,\nwhiteTiles=%d\n}", a.redTiles, a.blueTiles, a.greenTiles, a.whiteTiles);

    return 0;
}

//Define functions
int totalTiles(int redTiles, int blueTiles, int greenTiles, int whiteTiles)
{
    return redTiles + blueTiles + greenTiles + whiteTiles;
}
int maxTiles(struct mosaic * mosaicptr)
{
    int biggest = mosaicptr->redTiles;
    if (mosaicptr->blueTiles > biggest)
    {
        biggest = mosaicptr->blueTiles;   
    }
    if (mosaicptr->greenTiles > biggest)
    {
        biggest = mosaicptr->greenTiles;   
    }
    if (mosaicptr->whiteTiles > biggest)
    {
        biggest = mosaicptr->whiteTiles;   
    }
    return biggest;
}
void tripleTiles(struct mosaic * mosaicptr)
{
    mosaicptr->redTiles = mosaicptr->redTiles * 3;
    mosaicptr->blueTiles = mosaicptr->blueTiles * 3;
    mosaicptr->greenTiles = mosaicptr->greenTiles * 3;
    mosaicptr->whiteTiles = mosaicptr->whiteTiles * 3;
}
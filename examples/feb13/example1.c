//This example reads from a file using C functions.
//It was written by Andy Mitofsky
#include "../../se233.h"

int main()
{
    FILE * inFile;
    char buf[100];
    inFile=fopen("colors.txt","r");
    fgets(buf,100,inFile);
    printf("%s", buf);
    fclose(inFile);
    return 0;
}
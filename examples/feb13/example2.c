//This example reads using functions I write myself.
//It was written by Andy Mitofsky
#include "../../se233.h"

void readAndSmile(FILE * inFile);

int main()
{
    FILE * inFile;
    inFile=fopen("colors.txt","r");
    readAndSmile(inFile);
    fclose(inFile);
    return 0;
}

void readAndSmile(FILE * in)
{
    char buf[100];
    fgets(buf,100,in);
    printf(":) \n %s \n", buf);
    return;
}
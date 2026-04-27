//This example demonstrates writing to a file using C functions.
//It was written by Andy Mitofsky
//
#include "../se233.h"
int main()
{
	FILE * myFile;
	char buf[100];
	myFile=fopen("months.txt", "w");
	strcpy(buf, "February");
	fprintf(myFile, "%s", buf);
	fclose(myFile);
	return 0;
}

//This example demonstrates command line input. It was written by Andy Mitofsky
#include "../se233.h"


int main(int argc, char *argv[])
{
	printf("Argc is %d \n", argc);
	printf("argv[0] is %s \n", argv[0]);
	printf("argv[1] is %s \n", argv[1]);
	printf("argv[2] is %s \n", argv[2]);
	if(argc<2)
	{
		printf("you didn't include additional args.");
	}
	return 0;
}

//This example demonstrates manually allocating memory.
//It was written by Andy Mitofsky
#include "../se233.h"
int main()
{
	char *word;
	word = (char *)malloc(strlen("potato")+1);
	strcpy(word, "potato");
	printf("%s \n ", word);
	free(word);
	return 0;

}

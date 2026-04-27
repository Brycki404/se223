//This example demonstrates structs and pointers.
//It was written by Andy Mitofsky
#include "../se233.h"
struct dice {
	int diceSides;
	char diceColor[50];
	char material[50];
};

int main()
{
	struct dice d12;
	struct dice *dptr;
	dptr=&d12;
	strcpy(d12.material, "plastic");
	strcpy(d12.diceColor, "black");
	d12.diceSides=12;
	printf("The dice has %d sides \n", d12.diceSides);
	printf("The dice is made of %s \n", d12.material);
	(*dptr).diceSides=13;
	printf("The dice has been transformed into one with %d sides \n", d12.diceSides);
	dptr->diceSides=14;
	printf("It now has %d sides \n", d12.diceSides);

	return 0;

}

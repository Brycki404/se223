//This example demonstrates pointer use. 
//It was written by Andy Mitofsky.
#include "../se233.h"
int main()
{
	int value=15;
	int *vptr;   //Declare vptr to be a pointer
	vptr=&value; //Store Address of value in vptr
	printf("Value is %d \n", value);
	printf("Vptr is %p \n", vptr);
	printf("Contents of vptr is %d \n", *vptr);  //We're using the contents of operator
	printf("Address of value is %p \n", &value);
	printf("The address of vptr is %p \n", &vptr);

	return 0;
}


//This example demonstrates input
#include "../se233.h"

//Declare my function
double biggest(double a, double b, double c);

int main()
{
	char instuff[100];
	int x[5];
	int y[5]={4,5,7,2,3};
	int i=0;
	int countEnd;
        double ww=4.2;
	double vv=4.5;
	double uu=7.8;
	double outval=0;
	srand(time(NULL));
	printf("How many numbers should I use? \n");
	scanf ("%s", instuff);
	countEnd=atoi(instuff);
	while(i<countEnd)
	{
		printf("%d \n", i);
		i++;
	}
	
	for(i=0;i<5;i++)
	{
		x[i]=rand()%20;
		printf(" The array element is %d \n", x[i]);
	}
	outval=biggest(ww,uu,vv);
	printf("The biggest is %f \n", outval);




	return 0;
}


//Define my function
double biggest(double a, double b, double c)
{
	double big=a;
	if(b>big)
	{
		big=b;
	}
	if (c>big)
	{
		big=c;
	}
	return big;
}

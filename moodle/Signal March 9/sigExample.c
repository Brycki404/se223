//This example demonstrates writing our own signal handler
//It was written by Andy Mitofsky
#include "../se233.h"
//declare my signal handler function
static void sig_handler(int signo);

int main()
{

	//Register the signal handler
	if(signal(SIGINT, sig_handler)==SIG_ERR)
	{	perror("Signal error");
	}

	//Register for SIGFPE too
	if(signal (SIGFPE, sig_handler)==SIG_ERR)
	{
		perror("Signal error too");
	}
	printf("Hello \n");
	//pause();
//	raise(SIGINT);
	int x=0;
	int y=5;
	y=y/x;

	printf("Bye \n");

	return 0;
}


//define my signal handler here
static void sig_handler(int signo)
{

	printf("I caught signal sigint \n");
	psignal(signo, "the signal happened \n");
}

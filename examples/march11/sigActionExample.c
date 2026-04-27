//This example demonstrates writing our signal handler,
//and register it with sigaction. It was written by Andy Mitofsky
#include "../se233.h"

//declare the signal handler function
static void sig_handler (int signum);
int main()
{
	sigset_t set;
	struct sigaction act;

	//register the signal handler
	act.sa_handler=sig_handler;
	if(sigaction(SIGINT, &act, NULL))
	{
		perror("Sigaction error");
	}

	printf("Main says hello \n");
	pause();
	printf("Bye \n");


	return 0;
}

//define signal handler function
static void sig_handler(int signum)
{
	printf("I got the signal ");
	exit(0);

}
//This example comes straight from the pipe2 man page.
//This demonstrates forking a child and setting up a pipe. 
//The child reads and the parent writes.

#include "../se233.h"
int main(int argc, char * argv[])
{

	int pipefd[2];
	pid_t cpid;
	char buf;

	if(argc!=2)
	{
		fprintf(stderr, "USAGE: %s <string> \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Here we set up the pipe
	if(pipe(pipefd)==-1)
	{
		perror("Pipe error");
		exit(EXIT_FAILURE);
	}

	//Fork the child
	//
	cpid=fork();
	if(cpid==-1)
	{
		perror("Fork error");
		exit(EXIT_FAILURE);
	}
	if(cpid==0)
	{
		//We're in the child. We read from the pipe.
		close(pipefd[1]);//close unused end of pipe
		while(read(pipefd[0], &buf, 1)>0)
		{

			write(STDOUT_FILENO, &buf, 1);

		}
		write(STDOUT_FILENO, "\n",1);
		close(pipefd[0]);
		_exit(EXIT_SUCCESS);
	}
	else{
		//We're in the parent. We write
		close(pipefd[0]);//Close reading end of pipe
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);

	}







		}

//This example demonstrate shared memory between two processes. This is 
//the first one. I twas written by Andy Mitofsky. Before I started
//I created a empty file ./myshm
#include "../se233.h"
int main()
{
	int mid;
	char msg[30];
	char (*msgptr)[30];
	key_t mykey;

	strcpy(msg, "pineapple");

	//get the key
	mykey=ftok("./myshm", 18);
	if(mykey==-1)
	{
		perror("Ftok error");
	}
	//Set up the shared memory
	mid=shmget(mykey, 30, IPC_CREAT);
        if(mid==-1)
	{
		perror("shmget error");
	}	
	//Attach to memory
	msgptr=shmat(mid, 0,0);
	if(msgptr==-1)
	{
		perror("shmat error");
	}

	//Put info in memory
	strcpy(*msgptr, msg);
	//Detatch to memory
	if(shmdt(msgptr)==-1)
	{
		perror("Detatich error");
	}

	return 0;
}
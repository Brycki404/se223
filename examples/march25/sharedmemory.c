#include "../se233.h"

int main()
{
    //I think this is what was here, but I can't be sure. I don't have the original code.
    int mid;
    char msg2[30];
    char (*msgptr)[30];
    key_t mykey;

    strcpy(msg2, "pineapple");

    //get the key
    mykey=ftok("./myshm", 18);
    if (mykey==-1)
    {
        perror("ftok error");
    }
    //Set up the shared memory
    mid=shmget(mykey, 30, IPC_CREAT);
    if (mid==-1)
    {
        perror("shmget error");
    }
    //Attach to memory
    msgptr=shmat(mid, NULL, 0);
    if (msgptr==(void*)-1)
    {
        perror("shmat error");
    }
    //Put info in memory
    strcpy(*msgptr, msg2);
    //detatch
    if(shmdt(msgptr)==-1)
    {
        perror("Detatch error");
    }

    return 0;
}
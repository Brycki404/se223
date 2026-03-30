//This continues the shared memory example.
#include "../se233.h"

int main()
{
    int mid;
    char msg2[30];
    char (*msgptr2)[20];
    key_t mykey;

    //get the key
    mykey=ftok("./myshm", 18);
    if (mykey==-1)
    {
        perror("ftok error");
    }
    //setup with shmget
    mid=shmget(mykey, 30, IPC_CREAT);
    if (mid==-1)
    {
        perror("shmget error");
    }
    //attach
    msgptr2=shmat(mid, NULL, 0);
    if (msgptr2==(void*)-1)
    {
        perror("shmat error");
    }
    //use the memory
    printf("I read: %s \n", *msgptr2);
    //detatch
    if (shmdt(msgptr2)==-1)
    {
        perror("shmdt error");
    }
    return 0;
}
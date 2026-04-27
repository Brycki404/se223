#include "../se233.h"

// https://www.geeksforgeeks.org/operating-systems/ipc-using-message-queues/
// I used Geeksforgeeks to lookup the argments for the msg funtions and the ftok parameters

// Define the message structure
struct msgbuf {
    long mtype; // REQUIRED: message type must be > 0
    int sum; // our payload
};

int main()
{
    pid_t child, grandchild;
    key_t key;
    int msgid;

    printf("Original process PID: %d\n", getpid());

    int a, b;

    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);

    // Create message queue
    // Create a unique key for the message queue
    key = ftok(".", 'A');
    // "." means use current directory as a reference
    // 'A' is the project identifier (1 byte). Can be any character
    // ftok() combines these into a unqiue key_t value

    // Create or get the message queue
    msgid = msgget(key, IPC_CREAT | 0666);
    // IPC_CREATE means create queue if it doesn't exist
    // 0666 are the permissions : read/write for user, group, others
    // msgget() returns a queue ID (msgid)
    if (msgid < 0)
    {
        perror("msgget");
        exit(1);
    }

    child = fork();
    if (child < 0)
    {
        perror("child fork");
        exit(1);
    }

    if (child == 0)
    {
        printf("Child process PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        time_t now = time(NULL);
        printf("Child date/time: %s", ctime(&now));

        // Fork the grandchild
        grandchild = fork();
        if (grandchild < 0)
        {
            perror("grandchild fork");
            exit(1);
        }

        if (grandchild == 0)
        {
            printf("Grandchild process PID: %d, Parent PID: %d\n", getpid(), getppid());

            struct msgbuf message;

            // Receive message from original process
            // 1st arg: queue ID
            // 2nd arg: pointer to buffer
            // 3rd arg: size of payload
            // 4th arg: message type to recieve (1)
            // 5th arg: flags (0 = blocking)
            if (msgrcv(msgid, &message, sizeof(int), 1, 0) < 0)
            {
                perror("msgrcv");
                exit(1);
            }

            printf("Grandchildreceived sum: %d\n", message.sum);

            exit(0);
        }

        //Child waits for grandchild
        wait(NULL);
        exit(0);
    }

    // Prepare message
    struct msgbuf message;
    message.mtype = 1; // receiver can filter by type
    message.sum = a + b;

    // Send message to grandchild
    // 1st arg: queue ID
    // 2nd arg: pointer to message
    // 3rd arg: size of payload (not including mtype)
    // 4th arg: flags (0 = blocking)
    if (msgsnd(msgid, &message, sizeof(int), 0) < 0)
    {
        perror("msgsend");
        exit(1);
    }

    // Wait for child (which waits for grandchild)
    wait(NULL);

    // Remove the message queue
    msgctl(msgid, IPC_RMID, NULL);
    // IPC_RMID means remove the queue from the system
    // NULL means no extra options needed

    return 0;
}
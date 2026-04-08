#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_TEXT_SIZE 256

struct animal {
    char commonName[64];
    char latinName[64];
};

struct msgbuf_animal {
    long mtype;
    char mtext[MSG_TEXT_SIZE];
};

int main(void) {
    struct animal a;
    strncpy(a.commonName, "Red fox", sizeof(a.commonName));
    strncpy(a.latinName, "Vulpes vulpes", sizeof(a.latinName));

    const char *path = "./msgqueue.key";
    key_t key = ftok(path, 65);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        struct msgbuf_animal msg;

        msg.mtype = 1;
        snprintf(msg.mtext, sizeof(msg.mtext), "%s", a.commonName);
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd commonName");
            return 1;
        }

        msg.mtype = 2;
        snprintf(msg.mtext, sizeof(msg.mtext), "%s", a.latinName);
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd latinName");
            return 1;
        }

        wait(NULL);
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl IPC_RMID");
        }

    } else {
        struct msgbuf_animal recv_msg;

        if (msgrcv(msgid, &recv_msg, sizeof(recv_msg.mtext), 1, 0) == -1) {
            perror("msgrcv commonName");
            exit(1);
        }
        printf("Common name: %s\n", recv_msg.mtext);

        if (msgrcv(msgid, &recv_msg, sizeof(recv_msg.mtext), 2, 0) == -1) {
            perror("msgrcv latinName");
            exit(1);
        }
        printf("Latin name: %s\n", recv_msg.mtext);

        exit(0);
    }

    return 0;
}
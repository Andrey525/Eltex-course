#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define SERVER_KEY_PATH "./mqueue_key"
#define PROJECT_ID 'Z'
#define PERMISSIONS 0660 // S_IRUSR | S_IWUSR | S_IRGRP | S_IRGRP
#define BUF_SIZE 128

int server_queue_id;

struct message {
    int queue_id;
    char buf[BUF_SIZE];
};

struct msgbuf {
    long mtype;
    struct message message;
};

char *reverse(char str[], int size) {
    char tmp[BUF_SIZE];
    int i;
    for (i = 0; i < size; i++) {
        tmp[i] = str[size - i - 1];
    }
    tmp[size] = '\0';
    memset(str, 0, BUF_SIZE);
    strcpy(str, tmp);
    return str;
}

void remove_queue() {
    if ((msgctl(server_queue_id, IPC_RMID, NULL)) == -1) {
        perror("Server: msgctl remove queue");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main() {

    key_t server_queue_key;
    int client_queue_id;
    struct msgbuf send_msg_buf, recv_msg_buf;

    // создаем ключ для подключения к очереди соощений сервера
    if ((server_queue_key = ftok(SERVER_KEY_PATH, PROJECT_ID)) == -1) {
        perror("Server: ftok");
        exit(EXIT_FAILURE);
    }

    // создаем очередь сообщений сервера
    if ((server_queue_id = msgget(server_queue_key, IPC_CREAT | PERMISSIONS)) ==
        -1) {
        perror("Server: msgget server's queue id");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, remove_queue);

    while (1) {
        memset(&send_msg_buf, 0, sizeof(struct msgbuf));
        memset(&recv_msg_buf, 0, sizeof(struct msgbuf));

        if (msgrcv(server_queue_id, &recv_msg_buf, sizeof(struct message), 0,
                   0) == -1) {
            perror("Server: msgrcv");
            remove_queue();
            exit(EXIT_FAILURE);
        }

        strcpy(send_msg_buf.message.buf,
               reverse(recv_msg_buf.message.buf,
                       strlen(recv_msg_buf.message.buf)));

        client_queue_id = recv_msg_buf.message.queue_id;
        send_msg_buf.message.queue_id = server_queue_id;
        send_msg_buf.mtype = recv_msg_buf.mtype;

        if (msgsnd(client_queue_id, &send_msg_buf, sizeof(struct message), 0) ==
            -1) {
            perror("Server: msgsnd");
            remove_queue();
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
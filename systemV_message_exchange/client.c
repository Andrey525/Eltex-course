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

int my_queue_id;

struct message {
    int queue_id;
    char buf[BUF_SIZE];
};

struct msgbuf {
    long mtype;
    struct message message;
};

void clear_stdin() {
    while (getchar() != '\n')
        ;
}

void remove_queue() {
    if ((msgctl(my_queue_id, IPC_RMID, NULL)) == -1) {
        perror("Client: msgctl remove queue");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main() {

    key_t server_queue_key;
    int server_queue_id;
    struct msgbuf send_msg_buf, recv_msg_buf;

    // создаем ключ для подключения к очереди сообщений сервера
    if ((server_queue_key = ftok(SERVER_KEY_PATH, PROJECT_ID)) == -1) {
        perror("Clinet: ftok");
        exit(EXIT_FAILURE);
    }

    // подключаемся к очереди сообщений сервера
    if ((server_queue_id = msgget(server_queue_key, 0)) == -1) {
        perror("Client: msgget server's queue id");
        exit(EXIT_FAILURE);
    }

    // создаем свою приватную очередь
    if ((my_queue_id = msgget(IPC_PRIVATE, PERMISSIONS)) == -1) {
        perror("Client: msgget client's queue id");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, remove_queue);

    while (1) {
        memset(&recv_msg_buf, 0, sizeof(struct msgbuf));
        memset(&send_msg_buf, 0, sizeof(struct msgbuf));

        printf("Enter message (if you want to exit, enter \"exit\" without "
               "quotes):\n");
        fgets(send_msg_buf.message.buf, BUF_SIZE - 2, stdin);
        if (send_msg_buf.message.buf[strlen(send_msg_buf.message.buf) - 1] ==
            '\n') {
            send_msg_buf.message.buf[strlen(send_msg_buf.message.buf) - 1] =
                '\0';
        } else {
            clear_stdin();
        }
        if (strcmp(send_msg_buf.message.buf, "exit") == 0) {
            break;
        }

        send_msg_buf.mtype = 1;
        send_msg_buf.message.queue_id = my_queue_id;
        if (msgsnd(server_queue_id, &send_msg_buf, sizeof(struct message), 0) ==
            -1) {
            perror("Client: msgsnd");
            remove_queue();
            exit(EXIT_FAILURE);
        }

        if (msgrcv(my_queue_id, &recv_msg_buf, sizeof(struct message), 0, 0) ==
            -1) {
            perror("Client: msgrcv");
            remove_queue();
            exit(EXIT_FAILURE);
        }
        printf("Client: received message = \"%s\"\n", recv_msg_buf.message.buf);
    }

    remove_queue(my_queue_id);

    return 0;
}
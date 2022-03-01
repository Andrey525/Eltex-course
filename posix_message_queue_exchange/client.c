#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_MQ_NAME "/server_queue"
#define CLIENT_MQ_NAME "/client_queue"
#define PERMISSIONS 0660
#define MSG_SIZE 128

void clear_stdin() {
    while (getchar() != '\n')
        ;
}

void unlink_queue() {
    if (mq_unlink(CLIENT_MQ_NAME) == -1) {
        perror("Client: mq_unlink");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void close_and_unlink_queue(mqd_t mq_id) {
    //закрывам очередь сообщений
    if (mq_close(mq_id) == -1) {
        perror("Client: mq_close");
        exit(EXIT_FAILURE);
    }
    // удаляем имя очереди из системы.
    // сама очередь удалится, только в том случае, когда все использующие ее
    // процессы закроют ее
    if (mq_unlink(SERVER_MQ_NAME) == -1) {
        perror("Client: mq_unlink");
        exit(EXIT_FAILURE);
    }
}

int main() {
    mqd_t server_mq_id, client_mq_id;
    struct mq_attr attr;
    ssize_t bytes_read;
    char recv_buf[MSG_SIZE], send_buf[MSG_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 8;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    // присоединяемся к очереди сообщений, будем отправлять в нее
    if ((server_mq_id = mq_open(SERVER_MQ_NAME, O_WRONLY)) == (mqd_t)-1) {
        perror("Client: mq_open SERVER_MQ_NAME");
        exit(EXIT_FAILURE);
    }
    // создаем очередь сообщений, будем читать из нее
    if ((client_mq_id = mq_open(CLIENT_MQ_NAME, O_CREAT | O_RDONLY, PERMISSIONS,
                                &attr)) == (mqd_t)-1) {
        perror("Client: mq_open CLIENT_MQ_NAME");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, unlink_queue);

    while (1) {
        memset(send_buf, 0, sizeof(send_buf));
        memset(recv_buf, 0, sizeof(recv_buf));
        printf("Enter message (if you want to exit, enter \"exit\" without "
               "quotes):\n");
        fgets(send_buf, MSG_SIZE - 2, stdin);
        if (send_buf[strlen(send_buf) - 1] == '\n') {
            send_buf[strlen(send_buf) - 1] = '\0';
        } else {
            send_buf[MSG_SIZE - 1] = '\0';
            clear_stdin();
        }
        if (strcmp(send_buf, "exit") == 0) {
            break;
        }
        if (mq_send(server_mq_id, send_buf, sizeof(send_buf), 0) == -1) {
            perror("Client: mq_send");
            if (mq_unlink(CLIENT_MQ_NAME) == -1) {
                perror("Server: mq_unlink");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
        if ((bytes_read =
                 mq_receive(client_mq_id, recv_buf, MSG_SIZE, NULL) == -1)) {
            perror("Client: mq_receive");
            if (mq_unlink(CLIENT_MQ_NAME) == -1) {
                perror("Server: mq_unlink");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
        printf("Client: Received Message = \"%s\"\n", recv_buf);
    }

    //закрывам очередь сообщений, откуда читали (необязательно)
    if (mq_close(client_mq_id) == -1) {
        perror("Client: mq_close client_mq_id");
        exit(EXIT_FAILURE);
    }

    // удаляем имя очереди из системы.
    // сама очередь удалится, только в том случае, когда все использующие ее
    // процессы закроют ее
    if (mq_unlink(CLIENT_MQ_NAME) == -1) {
        perror("Server: mq_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
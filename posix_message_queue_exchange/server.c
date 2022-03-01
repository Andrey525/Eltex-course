#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_MQ_NAME "/server_queue"
#define CLIENT_MQ_NAME "/client_queue"
#define PERMISSIONS 0660
#define MSG_SIZE 128

char *reverse(char dest_str[], char src_str[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        dest_str[i] = src_str[size - i - 1];
    }
    dest_str[size] = '\0';
    return dest_str;
}

void unlink_queue() {
    if (mq_unlink(SERVER_MQ_NAME) == -1) {
        perror("Client: mq_unlink");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
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

    // создаем очередь сообщений, будем читать из нее
    if ((server_mq_id = mq_open(SERVER_MQ_NAME, O_CREAT | O_RDONLY, PERMISSIONS,
                                &attr)) == (mqd_t)-1) {
        perror("Server: mq_open");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, unlink_queue);

    while (1) {
        if ((bytes_read =
                 mq_receive(server_mq_id, recv_buf, MSG_SIZE, NULL) == -1)) {
            perror("Server: mq_receive");
            if (mq_unlink(SERVER_MQ_NAME) == -1) {
                perror("Server: mq_unlink");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
        printf("Server: Message = \"%s\"\n", recv_buf);
        reverse(send_buf, recv_buf, strlen(recv_buf));
        printf("Server: Reverse message = \"%s\"\n", send_buf);

        // присоединяемся к очереди сообщений, будем отправлять в нее
        if ((client_mq_id = mq_open(CLIENT_MQ_NAME, O_WRONLY)) == (mqd_t)-1) {
            perror("Server: mq_open CLIENT_MQ_NAME");
            if (mq_unlink(SERVER_MQ_NAME) == -1) {
                perror("Server: mq_unlink");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
        if (mq_send(client_mq_id, send_buf, sizeof(send_buf), 0) == -1) {
            perror("Server: mq_send");
            if (mq_unlink(SERVER_MQ_NAME) == -1) {
                perror("Server: mq_unlink");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
    }

    //закрывам очередь сообщений, откуда читали (необязательно)
    if (mq_close(server_mq_id) == -1) {
        perror("Server: mq_close");
        exit(EXIT_FAILURE);
    }
    // удаляем имя очереди из системы.
    // сама очередь удалится, только в том случае, когда все использующие ее
    // процессы закроют ее
    if (mq_unlink(SERVER_MQ_NAME) == -1) {
        perror("Server: mq_unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}
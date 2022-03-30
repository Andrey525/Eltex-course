#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int exchange_socket;
    struct sockaddr_in server_addr;
    int bytes_read, bytes_send;
    struct tm date;
    int n = 0;

    exchange_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (exchange_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(atoi(argv[1]));
    if (connect(exchange_socket, (struct sockaddr *)(&server_addr),
                sizeof(struct sockaddr_in)) == -1) {
        perror("CLIENT: connect");
        exit(1);
    }
    printf("CLIENT: Is ready for exhcanging\n");
    for (int i = 0; i < 5; i++) {
        bytes_send = send(exchange_socket, &n, sizeof(int), 0);
        if (bytes_send <= 0) {
            perror("CLIENT: send");
            exit(1);
        }
        bytes_read = recv(exchange_socket, &date, sizeof(struct tm), 0);
        if (bytes_read <= 0) {
            perror("CLIENT: recv");
            exit(1);
        }
        printf("CLIENT: Received message: %d_y %d_m %d_d %d_h %d_m %d_s\n",
               date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour,
               date.tm_min, date.tm_sec);
        sleep(2);
    }

    close(exchange_socket);
    return 0;
}
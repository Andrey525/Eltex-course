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
    socklen_t length;
    int bytes_read, bytes_send;
    struct tm date;
    int n;

    exchange_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (exchange_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(atoi(argv[1]));

    printf("CLIENT: Is ready for exchanging\n");
    bytes_send =
        sendto(exchange_socket, &n, sizeof(int), 0,
               (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_in));
    if (bytes_send <= 0) {
        perror("CLIENT: sendto");
        exit(1);
    }
    length = sizeof(struct sockaddr_in);
    bytes_read = recvfrom(exchange_socket, &date, sizeof(struct tm), 0,
                          (struct sockaddr *)(&server_addr), &length);
    if (bytes_read <= 0) {
        perror("CLIENT: recv");
        exit(1);
    }
    printf("CLIENT: Received message: %d_y %d_m %d_d %d_h %d_m %d_s\n",
           date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour, date.tm_min,
           date.tm_sec);

    close(exchange_socket);
    return 0;
}
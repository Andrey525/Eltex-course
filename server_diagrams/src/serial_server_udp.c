#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
    int exchanging_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t length;
    time_t sys_time;
    struct tm *date;
    int bytes_send, bytes_recv;
    int n;

    exchanging_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (exchanging_socket == -1) {
        perror("SERVER: socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;

    if (bind(exchanging_socket, (struct sockaddr *)(&server_addr),
             sizeof(struct sockaddr)) == -1) {
        perror("SERVER: bind");
        exit(1);
    }

    length = sizeof(struct sockaddr_in);
    if (getsockname(exchanging_socket, (struct sockaddr *)(&server_addr),
                    &length) < 0) {
        perror("SERVER: getsockname");
        exit(1);
    }
    printf("SERVER: port number - %d\n", ntohs(server_addr.sin_port));

    while (1) {
        length = sizeof(struct sockaddr_in);
        bytes_recv = recvfrom(exchanging_socket, &n, sizeof(int), 0,
                              (struct sockaddr *)(&client_addr), &length);
        if (bytes_recv <= 0) {
            perror("SERVER: recvfrom");
            exit(1);
        }
        printf("SERVER: Client address: %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        sys_time = time(NULL);
        date = localtime(&sys_time);
        date->tm_year += 1900;
        bytes_send = sendto(exchanging_socket, date, sizeof(struct tm), 0,
                            (struct sockaddr *)(&client_addr),
                            sizeof(struct sockaddr_in));
        if (bytes_send <= 0) {
            perror("SERVER: sendto");
            exit(1);
        }
    }

    return 0;
}

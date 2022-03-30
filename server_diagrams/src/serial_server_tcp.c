#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
    int listening_socket, exchanging_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t length;
    int bytes_send, bytes_recv;
    int n;
    time_t sys_time;
    struct tm *date;

    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        perror("SERVER: socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;

    if (bind(listening_socket, (struct sockaddr *)(&server_addr),
             sizeof(struct sockaddr_in)) < 0) {
        perror("SERVER: bind");
        exit(1);
    }

    length = sizeof(struct sockaddr_in);
    if (getsockname(listening_socket, (struct sockaddr *)(&server_addr),
                    &length) < 0) {
        perror("SERVER: getsockname");
        exit(1);
    }
    printf("SERVER: port number - %d\n", ntohs(server_addr.sin_port));

    if (listen(listening_socket, 5) < 0) {
        perror("SERVER: listen");
        exit(1);
    }

    while (1) {
        length = sizeof(struct sockaddr_in);
        exchanging_socket = accept(listening_socket,
                                   (struct sockaddr *)(&client_addr), &length);
        if (exchanging_socket == -1) {
            perror("SERVER: accept");
            exit(-1);
        }
        printf("SERVER: Client address: %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        while (1) {
            bytes_recv = recv(exchanging_socket, &n, sizeof(int), 0);
            if (bytes_recv <= 0) {
                // perror("SERVER: recv");
                break;
            }
            sys_time = time(NULL);
            date = localtime(&sys_time);
            date->tm_year += 1900;
            bytes_send = send(exchanging_socket, date, sizeof(struct tm), 0);
            if (bytes_send <= 0) {
                // perror("SERVER: send");
                break;
            }
        }
    }

    close(exchanging_socket);
    close(listening_socket);
    return 0;
}
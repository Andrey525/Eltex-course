#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int broadcast_socket;
    struct sockaddr_in broadcast_addr;
    int bytes_send;
    int n = 8888;
    int opt_flag = 1;

    broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcast_socket == -1) {
        perror("SERVER: socket");
        exit(1);
    }

    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_addr.sin_port = htons(4774);

    if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &opt_flag,
                   sizeof(int)) == -1) {
        perror("SERVER: setsockopt");
        exit(1);
    }

    bytes_send =
        sendto(broadcast_socket, &n, sizeof(int), 0,
               (struct sockaddr *)(&broadcast_addr), sizeof(struct sockaddr));

    if (bytes_send <= 0) {
        perror("SERVER: sendto");
        exit(1);
    }

    return 0;
}
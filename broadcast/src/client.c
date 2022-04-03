#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int broadcast_socket;
    struct sockaddr_in broadcast_addr;
    socklen_t length;
    int bytes_recv;
    int n;
    // int opt_flag = 1;

    broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcast_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_addr.sin_port = htons(4774);

    if (bind(broadcast_socket, (struct sockaddr *)(&broadcast_addr),
             sizeof(broadcast_addr)) == -1) {
        perror("CLIENT: bind");
        exit(1);
    }

    length = sizeof(broadcast_addr);

    bytes_recv = recvfrom(broadcast_socket, &n, sizeof(int), 0,
                          (struct sockaddr *)(&broadcast_addr), &length);

    if (bytes_recv <= 0) {
        perror("CLIENT: recvfrom");
        exit(1);
    }

    printf("CLIENT: Recv message: %d\n", n);

    return 0;
}
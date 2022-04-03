#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int multicast_socket;
    struct sockaddr_in multicast_addr;
    int bytes_send;
    int n = 8888;

    multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (multicast_socket == -1) {
        perror("SERVER: socket");
        exit(1);
    }

    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = htonl(inet_addr("224.0.0.1"));
    multicast_addr.sin_port = htons(4774);

    bytes_send =
        sendto(multicast_socket, &n, sizeof(int), 0,
               (struct sockaddr *)(&multicast_addr), sizeof(struct sockaddr));

    if (bytes_send <= 0) {
        perror("SERVER: sendto");
        exit(1);
    }

    return 0;
}
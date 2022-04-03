#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int multicast_socket;
    struct sockaddr_in addr;
    struct ip_mreqn mreq;
    socklen_t length;
    int bytes_recv;
    int n;

    multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (multicast_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(4774);

    if (bind(multicast_socket, (struct sockaddr *)(&addr), sizeof(addr)) ==
        -1) {
        perror("CLIENT: bind");
        exit(1);
    }

    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    mreq.imr_address.s_addr = htonl(INADDR_ANY);

    if (setsockopt(multicast_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
                   sizeof(struct ip_mreqn)) == -1) {
        perror("CLIENT: setsockopt");
        exit(1);
    }

    length = sizeof(addr);

    bytes_recv = recvfrom(multicast_socket, &n, sizeof(int), 0,
                          (struct sockaddr *)(&addr), &length);

    if (bytes_recv <= 0) {
        perror("CLIENT: recvfrom");
        exit(1);
    }

    printf("CLIENT: Recv message: %d\n", n);

    return 0;
}
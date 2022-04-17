#include <memory.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct PacketSend {
    struct udphdr udp_header;
    char mes[256];
};

struct PacketRecv {
    struct iphdr ip_header;
    struct udphdr udp_header;
    char mes[256];
};

int main(int argc, char *argv[]) {
    int exchange_socket;
    struct sockaddr_in server_addr;
    socklen_t length;
    int bytes_send, bytes_recv;

    struct PacketSend packet_send;
    struct PacketRecv packet_recv;

    exchange_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (exchange_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    packet_send.udp_header.source = htons(8080);
    packet_send.udp_header.dest = htons(atoi(argv[1]));
    packet_send.udp_header.len = htons(sizeof(struct PacketSend));
    packet_send.udp_header.check = 0;
    strcpy(packet_send.mes, "HELLO");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    printf("CLIENT: Is ready for exchanging\n");
    bytes_send =
        sendto(exchange_socket, &packet_send, sizeof(struct PacketSend), 0,
               (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_in));
    if (bytes_send <= 0) {
        perror("CLIENT: sendto");
        exit(1);
    }
    while (1) {
        length = sizeof(struct sockaddr_in);
        memset(&packet_recv, 0, sizeof(struct PacketRecv));
        bytes_recv =
            recvfrom(exchange_socket, &packet_recv, sizeof(struct PacketRecv),
                     0, (struct sockaddr *)(&server_addr), &length);
        if (bytes_recv <= 0) {
            perror("CLIENT: recv");
            exit(1);
        }
        // сам принял свой трафик
        if (packet_recv.udp_header.dest == htons(atoi(argv[1]))) {
            printf("mes = %s\n", packet_recv.mes);
        }
        // принял пакет от сервера
        if (packet_recv.udp_header.dest == htons(8080)) {
            printf("mes = %s\n", packet_recv.mes);
        }
    }
    close(exchange_socket);
    return 0;
}
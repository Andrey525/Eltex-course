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

struct Packet {
    struct iphdr ip_header;
    struct udphdr udp_header;
    char mes[256];
};

int main(int argc, char *argv[]) {
    int exchange_socket;
    struct sockaddr_in server_addr;
    socklen_t length;
    int bytes_send, bytes_recv;
    int val = 1;

    struct Packet packet;

    exchange_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (exchange_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    if (setsockopt(exchange_socket, IPPROTO_IP, IP_HDRINCL, &val,
                   sizeof(val)) == -1) {
        perror("CLIENT: setsockopt");
        exit(1);
    }
    memset(&packet, 0, sizeof(struct Packet));

    packet.ip_header.version = 4;
    packet.ip_header.ihl = 5; // 4-ч байтные слова 4*5 = 20 байт
    packet.ip_header.tos = 0;
    packet.ip_header.tot_len = htons(sizeof(struct Packet));
    packet.ip_header.id = 0; // любой, можно 0 (система автоматом выберет)
    packet.ip_header.frag_off = 0;
    packet.ip_header.ttl = 64;
    packet.ip_header.protocol = IPPROTO_UDP; // udp
    packet.ip_header.check = 0; // система посчиатет
    packet.ip_header.saddr = htonl(INADDR_LOOPBACK);
    packet.ip_header.daddr = htonl(INADDR_LOOPBACK);

    packet.udp_header.source = htons(8080);
    packet.udp_header.dest = htons(atoi(argv[1]));
    packet.udp_header.len =
        htons(sizeof(packet.mes) + sizeof(packet.udp_header));
    packet.udp_header.check = 0;
    strcpy(packet.mes, "HELLO");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(atoi(argv[1]));

    printf("CLIENT: Is ready for exchanging\n");
    bytes_send =
        sendto(exchange_socket, &packet, sizeof(struct Packet), 0,
               (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_in));
    if (bytes_send <= 0) {
        perror("CLIENT: sendto");
        exit(1);
    }
    while (1) {
        length = sizeof(struct sockaddr_in);
        memset(&packet, 0, sizeof(struct Packet));
        bytes_recv = recvfrom(exchange_socket, &packet, sizeof(struct Packet),
                              0, (struct sockaddr *)(&server_addr), &length);
        if (bytes_recv <= 0) {
            perror("CLIENT: recv");
            exit(1);
        }
        // сам принял свой трафик
        if (packet.udp_header.dest == htons(atoi(argv[1]))) {
            printf("mes = %s\n", packet.mes);
        }
        // принял пакет от сервера
        if (packet.udp_header.dest == htons(8080)) {
            printf("mes = %s\n", packet.mes);
        }
    }
    close(exchange_socket);
    return 0;
}
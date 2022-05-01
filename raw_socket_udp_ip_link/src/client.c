#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <memory.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct Packet {
    struct ethhdr eth_header;
    struct iphdr ip_header;
    struct udphdr udp_header;
    char mes[256];
};

uint16_t checksum(struct iphdr iphdr) {
    int csum = 0;
    int *ptr = (int *)&iphdr;
    for (int i = 0; i < iphdr.ihl; i++) {
        csum += *ptr;
        ptr++;
    }
    csum = (csum & 0xffff) + (csum >> 16);
    return (uint16_t)~csum;
}

int main(int argc, char *argv[]) {
    int exchange_socket;
    struct sockaddr_ll server_addr;
    socklen_t length;
    int bytes_send, bytes_recv;

    struct Packet packet;

    exchange_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (exchange_socket == -1) {
        perror("CLIENT: socket");
        exit(1);
    }

    memset(&packet, 0, sizeof(struct Packet));

    server_addr.sll_family = AF_PACKET;
    server_addr.sll_protocol = htons(ETH_P_ALL);
    server_addr.sll_ifindex = if_nametoindex("enp3s0");
    // server_addr.sll_hatype = ARPHRD_ETHER;
    // server_addr.sll_pkttype = PACKET_HOST;
    server_addr.sll_halen = ETH_ALEN;
    server_addr.sll_addr[0] = 0xd8;
    server_addr.sll_addr[1] = 0x5e;
    server_addr.sll_addr[2] = 0xd3;
    server_addr.sll_addr[3] = 0x76;
    server_addr.sll_addr[4] = 0x76;
    server_addr.sll_addr[5] = 0xc4;
    server_addr.sll_addr[6] = 0x00;
    server_addr.sll_addr[7] = 0x00;

    packet.eth_header.h_dest[0] = 0x78;
    packet.eth_header.h_dest[1] = 0x45;
    packet.eth_header.h_dest[2] = 0x61;
    packet.eth_header.h_dest[3] = 0xc1;
    packet.eth_header.h_dest[4] = 0x45;
    packet.eth_header.h_dest[5] = 0x9d;
    packet.eth_header.h_proto = ETH_P_802_3;
    memcpy(packet.eth_header.h_source, server_addr.sll_addr, ETH_ALEN);

    packet.ip_header.version = 4;
    packet.ip_header.ihl = 5; // 4-х байтные слова 4*5 = 20 байт
    packet.ip_header.tos = 0;
    packet.ip_header.tot_len =
        htons(sizeof(struct Packet) - sizeof(struct ethhdr));
    packet.ip_header.id = 0; // любой, можно 0 (система автоматом выберет)
    packet.ip_header.frag_off = 0;
    packet.ip_header.ttl = 64;
    packet.ip_header.protocol = IPPROTO_UDP; // udp
    packet.ip_header.check = checksum(packet.ip_header);
    inet_pton(AF_INET, "192.168.0.101", &(packet.ip_header.saddr));
    inet_pton(AF_INET, "192.168.0.104", &(packet.ip_header.daddr));

    packet.udp_header.source = htons(8080);
    packet.udp_header.dest = htons(atoi(argv[1]));
    packet.udp_header.len =
        htons(sizeof(packet.mes) + sizeof(packet.udp_header));
    packet.udp_header.check = 0; // можно не считать
    strcpy(packet.mes, "HELLO");

    printf("CLIENT: Is ready for exchanging\n");
    bytes_send =
        sendto(exchange_socket, &packet, sizeof(struct Packet), 0,
               (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_ll));
    if (bytes_send <= 0) {
        perror("CLIENT: sendto");
        exit(1);
    }
    while (1) {
        length = sizeof(struct sockaddr_ll);
        memset(&packet, 0, sizeof(struct Packet));
        bytes_recv = recvfrom(exchange_socket, &packet, sizeof(struct Packet),
                              0, (struct sockaddr *)(&server_addr), &length);
        if (bytes_recv <= 0) {
            perror("CLIENT: recv");
            exit(1);
        }
        // сам принял свой трафик
        if (packet.udp_header.dest == htons(atoi(argv[1]))) {
            printf("my mes = %s\n", packet.mes);
        }
        // принял пакет от сервера
        if (packet.udp_header.dest == htons(8080)) {
            printf("mes from server = %s\n", packet.mes);
        }
    }
    close(exchange_socket);
    return 0;
}
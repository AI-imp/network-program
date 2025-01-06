#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define ErrExit(msg) do {\
    perror(msg); \
    exit(EXIT_FAILURE);\
} while(0)

typedef struct sockaddr Addr;
typedef struct sockaddr_in Addr_in;

int main(int argc, char *argv[]) {
    int fd = -1;
    Addr_in peeraddr;
    struct ip_mreq mreq;
    char buf[BUFSIZ] = {};

    /*参数检查*/
    if (argc < 3) {
        fprintf(stderr, "%s <multicast_addr> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*创建套接字*/
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        ErrExit("socket");
    }

    /*设置多播组地址*/
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(atoi(argv[2]));
    if (!inet_aton(argv[1], &peeraddr.sin_addr)) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    /*加入多播组*/
    mreq.imr_multiaddr.s_addr = peeraddr.sin_addr.s_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        ErrExit("setsockopt");
    }

    /*发送数据*/
    while (1) {
        fgets(buf, BUFSIZ, stdin);
        sendto(fd, buf, strlen(buf) + 1, 0, (Addr *)&peeraddr, sizeof(peeraddr));
    }

    return 0;
}
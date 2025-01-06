#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
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
    socklen_t peerlen = sizeof(peeraddr);
    char buf[BUFSIZ] = {};

    /*参数检查*/
    if (argc < 3) {
        fprintf(stderr, "%s <multiaddr> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*创建套接字*/
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        ErrExit("socket");
    }

    /*允许广播*/
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    /*设置通信结构体*/
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(atoi(argv[2]));
    if (!inet_aton(argv[1], &peeraddr.sin_addr)) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fgets(buf, BUFSIZ, stdin);
        sendto(fd, buf, strlen(buf) + 1, 0, (Addr *)&peeraddr, peerlen);
    }

    return 0;
}
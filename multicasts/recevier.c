#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>

#define ErrExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

typedef struct sockaddr Addr;
typedef struct sockaddr_in Addr_in;

int main(int argc, char *argv[]) {
    int fd = -1;
    Addr_in myaddr, peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    struct ip_mreqn mreq;
    char buf[BUFSIZ] = {};

    /*参数检查*/
    if (argc < 3) {
        fprintf(stderr, "%s <addr> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*创建套接字*/
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        ErrExit("socket");
    }
    /*加入多播组*/
    bzero(&mreq,sizeof(mreq));
    if(!inet_aton(argv[1],&mreq.imr_multiaddr) ){
        fprintf(stderr,"Invalid address\n");
        exit(EXIT_FAILURE);}
    if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) <0){
        perror("setsockopt");
        exit(0);
    }
    /*设置通信结构体*/
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(atoi(argv[2]));
    if (!inet_aton(argv[1], &myaddr.sin_addr)) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    /*绑定通信结构体*/
    if (bind(fd, (Addr *)&myaddr, sizeof(Addr_in)) ) {
        ErrExit("bind");
    }

    while (1) {
        recvfrom(fd, buf, BUFSIZ, 0, (Addr *)&peeraddr, &peerlen);
        printf("[%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), buf);
    }
    return 0;
}

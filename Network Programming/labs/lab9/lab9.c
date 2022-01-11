#include "unp.h"
#include <netinet/tcp.h>


int main(int argc, char** argv) {
    int socket_fd;
    struct sockaddr_in srvaddr;
    int mss = 0, rec_buf = 0;
    unsigned int mss_len = sizeof(mss);
    unsigned int rec_len = sizeof(rec_buf);

    if (argc != 2) {
        perror("Insufficient Arguments!\n");
        return EXIT_FAILURE;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);//TCP socket
    if (socket_fd == -1) {
        perror("socket_fd FAILURE!\n");
        return EXIT_FAILURE;
    }

    if (getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF,(void *)&rec_buf, &rec_len)) {
        perror("getsockopt SO_RCVBUF before connect FAILUER!\n");
        return EXIT_FAILURE;
    }

    if (getsockopt(socket_fd, IPPROTO_TCP, TCP_MAXSEG, &mss, &mss_len)) {
        perror("getsockopt TCP_MAXSEG before connect FAILUER!\n");
        return EXIT_FAILURE;
    }
    printf("before connect, MSS: %d, receive buffer size:%d Byte\n", mss, rec_buf);

//---------------------
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
    srvaddr.sin_port = htons(80);

    if(connect(socket_fd, (void*)&srvaddr, sizeof(srvaddr))) {
        perror("connect FAILURE!\n");
        return EXIT_FAILURE;
    }

    if (getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF,(void *)&rec_buf, &rec_len)) {
        perror("getsockopt SO_RCVBUF after connect FAILUER!\n");
        return EXIT_FAILURE;
    }

    if (getsockopt(socket_fd, IPPROTO_TCP, TCP_MAXSEG, &mss, &mss_len)) {
        perror("getsockopt TCP_MAXSEG after connect FAILUER!\n");
        return EXIT_FAILURE;
    }

    printf("after connect, MSS: %d, receive buffer size:%d Byte\n", mss, rec_buf);

    close(socket_fd);
    return EXIT_SUCCESS;
}
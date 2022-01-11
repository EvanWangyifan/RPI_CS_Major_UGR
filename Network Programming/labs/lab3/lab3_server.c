//#include "../unpv13e-master/lib/unp.h" // 
#include "unp.h"
#define Maxline 517

int main(int argc, char * argv[]) {
  setbuf(stdout, NULL);
  int maxfd, connfd, lfd, port;
  char buf[Maxline];
  //fd_set rset,allset;
  struct sockaddr_in cliaddr,servaddr;
  socklen_t addr_len = sizeof(cliaddr);

  if (argc != 2) {
    perror("INVALID ARGUMENTS!");
    return EXIT_FAILURE;
  }

  lfd = Socket(AF_INET, SOCK_STREAM, 0);
  port = atoi(argv[1]) + 9877;

  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(port);

  Bind(lfd, (SA*)&servaddr, sizeof(servaddr));
  Listen(lfd, 1);

  connfd = Accept(lfd, (SA*)&cliaddr, (socklen_t *)&addr_len);
  printf("Accepted connection\n");
  while(!feof(stdin)){
    memset(buf, 0, sizeof(buf));
    scanf("%s",buf);
    buf[strlen(buf)] = '\n';
    Writen(connfd, buf, strlen(buf));
  }
  Close(connfd);
  printf("Shutting down due to EOF\n");
}
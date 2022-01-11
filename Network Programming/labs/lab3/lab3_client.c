//#include "../unpv13e-master/lib/unp.h" // 
#include "unp.h"
#define Maxline 517

int port_checker[5] = {0, 0, 0, 0, 0};
int main(int argc, char * argv[]) {
  int n, connfd, sockfd, port, ready;
  char buf[Maxline];
  int connections[5], check_conn[5];
  struct sockaddr_in cliaddr,servaddr;
  for (int i = 0; i < 5; ++i)
  {
    check_conn[i] = -1;
  }
  fd_set rset;
  FD_ZERO(&rset);
  for(;;){
    FD_SET(fileno(stdin), &rset);
    for(int i = 0; i < 5; i++) {
      if(check_conn[i] != -1)
        FD_SET(connections[i], &rset);
    }
    ready = select(8, &rset,NULL,NULL,NULL);
    if (FD_ISSET(fileno(stdin), &rset)) {
      char tmp[Maxline];
      Fgets(tmp, Maxline, stdin);
      int port = atoi(tmp);
      printf("%d Connected\n", port);
      int i;
      for (i = 0; i < 5; i++) {
        if (check_conn[i]==-1) {
          check_conn[i] = 1;
          connections[i] = Socket(AF_INET, SOCK_STREAM, 0);
          bzero(&servaddr,sizeof(servaddr));
          servaddr.sin_family      = AF_INET;
          servaddr.sin_port        = htons(port);
          servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
          
          Connect(connections[i], (SA *) &servaddr, sizeof(servaddr));
          port_checker[i] = port;
          break;
        }
      }
    }
    for (int i = 0; i < 5; i++) {
      if (check_conn[i]!=-1){
        struct sockaddr_in localaddr;
        socklen_t len = sizeof(localaddr);
        int ret = getsockname(connections[i], (SA*)&localaddr, &len);
        if(FD_ISSET(connections[i], &rset)){
          memset(buf, 0, sizeof(buf));
          if(Readline(connections[i], buf, Maxline) == 0) {
            printf("Server on %d closed\n", port_checker[i]);
            check_conn[i] = -1;
            close(connections[i]);
          }
          else{
            printf("%d : %s", port_checker[i], buf);
          }
        }
      }
    }
  }
}


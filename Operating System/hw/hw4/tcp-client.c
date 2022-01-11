/* tcp-client.c */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXBUFFER 8192
#define MAXNUMBER 100

int main(int argc, char **argv) {
  // disbable buffered output
  setvbuf(stdout, NULL, _IONBF, 0);

  // check amount of arguments
  if (argc < 4) {
    fprintf(stderr, "Invalid argument(s)\n");
    fprintf(stderr, "USAGE: a.out <server-hostname> <server-port> <n> "
                    "<int-value-1> ... <int-value-n>\n");
    exit(EXIT_FAILURE);
  }

  // get domain and port and numbers
  char *domain = argv[1];
  int port = atoi(argv[2]);
  int numbers[MAXNUMBER];
  int numberstosend[MAXNUMBER];
  numbers[0] = atoi(argv[3]);
  numberstosend[0] = htonl(numbers[0]);
  for (int i = 1; i <= numbers[0]; i++) {
    numbers[i] = atoi(argv[3 + i]);
    numberstosend[i] = htonl(numbers[i]);
  }

  /* create TCP client socket (endpoint) */
  int sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    fprintf(stderr, "ERROR: socket() failed");
    exit(EXIT_FAILURE);
  }

  struct hostent *hp = gethostbyname(domain);
  if (hp == NULL) {
    fprintf(stderr, "ERROR: gethostbyname() failed\n");
    return EXIT_FAILURE;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  memcpy((void *)&server.sin_addr, (void *)hp->h_addr, hp->h_length);
  server.sin_port = htons(port);

  printf("CLIENT: Connected to server\n");
  if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("ERROR: connect() failed");
    return EXIT_FAILURE;
  }

  // send numbers to server
  int n = write(sd, (char *)numberstosend,
                (numbers[0] + 1) * 4); /* or send()/recv() */
  if (n == -1) {
    perror("ERROR: write() failed");
    close(sd);
    return EXIT_FAILURE;
  }
  if (numbers[0] <= 1)
    printf("CLIENT: Sending %d integer value\n", numbers[0]);
  else
    printf("CLIENT: Sending %d integer values\n", numbers[0]);

  // read number from server
  int nresp;
  n = read(sd, (char *)&nresp, 4); /* BLOCKING */
  if (n == -1) {
    perror("ERROR: read() failed");
    close(sd);
    return EXIT_FAILURE;
  } else if (n == 0) {
    fprintf(stderr,
            "ERROR: CLIENT: Rcvd no data; also, server socket was closed\n");
  } else {
    printf("CLIENT: Rcvd response %d\n", ntohl(nresp));
  }

  // read message from server
  char buffer[MAXBUFFER];
  n = read(sd, buffer, MAXBUFFER - 1); /* BLOCKING */
  if (n == -1) {
    perror("ERROR: read() failed");
    close(sd);
    return EXIT_FAILURE;
  } else if (n == 0) {
    fprintf(stderr,
            "ERROR: CLIENT: Rcvd no data; also, server socket was closed\n");
  } else /* n > 0 */
  {
    buffer[n] = '\0';
    printf("CLIENT: Rcvd response \"%s\"\n", buffer);
  }

  printf("CLIENT: Disconnected from server\n");

  close(sd);

  return EXIT_SUCCESS;
}

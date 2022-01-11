#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char** argv) {
    setbuf(stdout, NULL);
	if ( argc != 2 ) {
		perror("invalid argument\n" );
		return EXIT_FAILURE;
	}
    char* name = argv[1];
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
	hints.ai_protocol = 0;
    status = getaddrinfo(name, NULL, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo failed, interpreting return status code: %s\n", gai_strerror(status));
        perror("getaddrinfo failed, printing errno");
        exit(EXIT_FAILURE);
    }
    struct addrinfo* res;
    
    for(res = servinfo; res != NULL; res = res->ai_next) {
        char addr[128];
        if(res->ai_family == AF_INET) {
            inet_ntop(AF_INET, &((struct sockaddr_in*)res->ai_addr)->sin_addr, addr, sizeof(addr));
        }
        else if(res->ai_family == AF_INET6) {
            inet_ntop(AF_INET6, &((struct sockaddr_in6*)res->ai_addr)->sin6_addr, addr, sizeof(addr));
        }
        printf("%s\n", addr);
    }
    freeaddrinfo(servinfo);

	return EXIT_SUCCESS;
}
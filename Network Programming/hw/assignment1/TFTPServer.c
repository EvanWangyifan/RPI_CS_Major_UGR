#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "unp.h"
//#include "../unpv13e-master/lib/unp.h"

#define MAXPACK 516
#define MAXDATA 512
#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5

//global variable used in signal processing
int SERVER_SOCKET;
unsigned short int CURRENT_BLOCK;
int TIME_OUT_COUNTER = 0;
struct sockaddr_in CURRENT_CLI;
char CURRENT_DATA[MAXPACK]; 

void check_ack(char* msg, int block_num);
void RRQ_time_out_handler(int signum);
void WRQ_time_out_handler(int signum);
void send_ack(int socket, unsigned short int block_num);
void send_error(int socket, unsigned short int error_code, char* error_msg);
void send_data();
void read_request(int port, char* file_name);
void write_request(int port, char* file_name);


int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    //Set the parent socket
    int sockfd;
    char buffer[MAXPACK];
    struct sockaddr_in servaddr;
    
    //Set the port
    int start_port = atoi(argv[1]);
    int end_port = atoi(argv[2]);
    int current_port = start_port + 1;

    printf("Parent: start port:%d, end port:%d\n", start_port, end_port);

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Parent: socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&CURRENT_CLI, 0, sizeof(CURRENT_CLI));

    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(start_port);
    //Try bind the socket to the first port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Parent: bind failed");
        return EXIT_FAILURE;
    }

    printf("Parent: Socket %d bind to port %d\n", sockfd, ntohs(servaddr.sin_port));

    while(1) {
        int len, n;
        len = sizeof(CURRENT_CLI);
        //Waiting for connection
        n = Recvfrom(sockfd, (char *)buffer, MAXPACK, 
                MSG_WAITALL, ( struct sockaddr *) &CURRENT_CLI,
                &len);
        //Decompose the msg
        unsigned short int opcode;
        unsigned short int * opcode_ptr;
        opcode_ptr = (unsigned short int *) buffer;
        opcode = ntohs(*opcode_ptr);
        printf("Parent: receive packet with opcode %d\n", opcode);
        if(opcode != RRQ && opcode != WRQ) {
            send_error(sockfd, 4, "Invalid operation");
        }
        else{
            pid_t pid = fork();
            if(opcode == RRQ && pid == 0) {
                //read request
                printf("Parent: Child start read request on port %d\n", current_port);
                read_request(current_port, buffer + 2);
                close(sockfd);
                return 0;
            }
            if(opcode == WRQ && pid == 0) {
                //write request
                printf("Parent: Child start write request on port %d\n", current_port);
                write_request(current_port, buffer + 2);
                close(sockfd);
                return 0;
            }
            current_port++;
        }
    }
}

void check_ack(char* msg, int block_num) {
    //Check if the ACK violates TFTP protocol
    unsigned short int * opcode_ptr = (unsigned short int *) msg;
    unsigned short int opcode = ntohs(*opcode_ptr);
    unsigned short int receive_block_num = ntohs(*(opcode_ptr+1));
    printf("Child: opcode %d, blocknum %d\n", opcode, receive_block_num);
    if(opcode != ACK || receive_block_num != block_num) {
        printf("Child: Wrong Ack packet, abort!\n");
        send_error(SERVER_SOCKET, 4, "Illegal TFTP operation.");
        abort();
    }
}

void RRQ_time_out_handler(int signum) {
    //Read request timeout: resend the data packet
    if(signum == SIGALRM) {
        if(TIME_OUT_COUNTER < 10) {
            printf("Child: time out, resend, count: %d\n", TIME_OUT_COUNTER);
            send_data();
            alarm(0);
            alarm(1);
            TIME_OUT_COUNTER += 1;
        }
        else{
            printf("Child: Time out for %d second, abort\n", TIME_OUT_COUNTER);
            TIME_OUT_COUNTER = 0;
            abort();
        }
    }
}

void WRQ_time_out_handler(int signum) {
    //Write request timeout: resend the ACK packet
    if(signum == SIGALRM) {
        if(TIME_OUT_COUNTER < 10) {
            printf("Child: time out, resend ack, count: %d\n", TIME_OUT_COUNTER);
            send_ack(SERVER_SOCKET, CURRENT_BLOCK);
            alarm(0);
            alarm(1);
            TIME_OUT_COUNTER += 1;
        }
        else{
            printf("Child: Time out for %d second, abort\n", TIME_OUT_COUNTER);
            TIME_OUT_COUNTER = 0;
            abort();
        }
    }
}

void send_ack(int socket, unsigned short int block_num) {
    //Send the ACK to client
    char buffer[MAXPACK];
    int len = sizeof(CURRENT_CLI);
    unsigned short int * opcode_ptr;
    opcode_ptr = (unsigned short int *) buffer;
    *opcode_ptr = htons(ACK);
    *(opcode_ptr+1) = htons(block_num);
    printf("Child: Send ACK, with block_num %d\n", block_num);
    Sendto(socket, (const char *)buffer, sizeof(buffer), 
                MSG_CONFIRM, (const struct sockaddr *) &CURRENT_CLI,
                    len);
}

void send_error(int socket, unsigned short int error_code, char* error_msg) {
    //Send error msg
    char buffer[MAXPACK];
    int len = sizeof(CURRENT_CLI);
    unsigned short int * opcode_ptr;
    opcode_ptr = (unsigned short int *) buffer;
    *opcode_ptr = htons(ERROR);
    *(opcode_ptr + 1) = htons(error_code);
    memcpy(buffer+4, error_msg, strlen(error_msg));
    printf("Child: send errorcode %d, msg %s\n", error_code, error_msg);
    Sendto(socket, (const char *)buffer, sizeof(buffer), 
        MSG_CONFIRM, (const struct sockaddr *) &CURRENT_CLI,
            len);
}

void send_data() {
    //Send the DATA packet
    unsigned short int * opcode_ptr = (unsigned short int *) CURRENT_DATA;
    printf("Child: Buffer sent(size %ld)\n", strlen(CURRENT_DATA+4)+4);
    Sendto(SERVER_SOCKET, (const char *)CURRENT_DATA, strlen(CURRENT_DATA+4)+4, 
        MSG_CONFIRM, (const struct sockaddr *) &CURRENT_CLI,
            sizeof(CURRENT_CLI));
}

void read_request(int port, char* file_name) {
    // Make a new server socket
    char ack_buffer[MAXPACK];
    CURRENT_BLOCK = 1;
    struct sockaddr_in servaddr;
    SERVER_SOCKET = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
    bind(SERVER_SOCKET, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Child: Socket %d bind to port %d\n", SERVER_SOCKET, ntohs(servaddr.sin_port));
    unsigned short int * opcode_ptr;
    //Check if the file exist
    printf("%s\n", file_name);
    int fd = open(file_name, O_RDONLY);
    if(fd < 0) {
        perror("Child: File not found\n");
        send_error(SERVER_SOCKET, 1, "File not found.");
        abort();
    }
    int byte_read;
    //Send the data packet
    do {
        int n = 0;
        int len = sizeof(CURRENT_CLI);
        Signal(SIGALRM, RRQ_time_out_handler);
        alarm(0);
        memset(ack_buffer, 0, sizeof(ack_buffer));
        memset(CURRENT_DATA, 0, sizeof(CURRENT_DATA));
        opcode_ptr = (unsigned short int *) CURRENT_DATA;
        *opcode_ptr = htons(3);
        *(opcode_ptr+1) = htons(CURRENT_BLOCK);
        printf("Child: Block : %d\n",  ntohs(*(opcode_ptr+1)));
        byte_read = read(fd, CURRENT_DATA+4, 512);
        printf("Child: Read %d bytes\n", byte_read);
        send_data();
        alarm(1);
        n = Recvfrom(SERVER_SOCKET, (char *)ack_buffer, MAXPACK, 
                0, ( struct sockaddr *) &CURRENT_CLI,
                &len);
        alarm(0);
        printf("Child: Receive ACK\n");
        check_ack(ack_buffer, CURRENT_BLOCK);
        CURRENT_BLOCK += 1;
    } while(byte_read == 512);
}

void write_request(int port, char* file_name) {
    // Make a new server socket
    char write_buffer[MAXPACK];
    CURRENT_BLOCK = 0;
    struct sockaddr_in servaddr;
    SERVER_SOCKET = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
    bind(SERVER_SOCKET, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Child: Socket %d bind to port %d\n", SERVER_SOCKET, ntohs(servaddr.sin_port));
    unsigned short int * opcode_ptr;
    printf("%s\n", file_name);
    //Check if the file exist
    int fd = open(file_name, O_RDONLY);
    if(fd > 0) {
        send_error(SERVER_SOCKET, 6, "File already exists.");
        abort();
    }
    close(fd);
    fd = open(file_name, O_WRONLY | O_CREAT);
    send_ack(SERVER_SOCKET, CURRENT_BLOCK);
    CURRENT_BLOCK += 1;
    int n = 0;
    //Send the ACK packet and write to the file
    do {
        int len = sizeof(CURRENT_CLI);
        Signal(SIGALRM, WRQ_time_out_handler);
        alarm(0);
        memset(write_buffer, 0, sizeof(write_buffer));
        alarm(1);
        n = Recvfrom(SERVER_SOCKET, (char *)write_buffer, MAXPACK, 
                0, ( struct sockaddr *) &CURRENT_CLI,
                &len);
        alarm(0);
        printf("Child: Receive %d bytes\n", n);
        unsigned short int * opcode_ptr;
        opcode_ptr = (unsigned short int *) write_buffer;
        if(ntohs(*opcode_ptr) == 3) {
            printf("Child: block_num receive %d\n", ntohs((*opcode_ptr+1)));
            printf("Child: local block_num %d\n", CURRENT_BLOCK);
            if(ntohs(*(opcode_ptr+1)) == CURRENT_BLOCK) {
                int byte_write = write(fd, write_buffer+4, n-4);
                printf("Child: write %d bytes", byte_write);
            }
        }
        else{
            send_error(SERVER_SOCKET, 4, "Illegal TFTP operation.");
            abort();
        }
        send_ack(SERVER_SOCKET, CURRENT_BLOCK);
        CURRENT_BLOCK += 1;
    } while(n == 516);
}
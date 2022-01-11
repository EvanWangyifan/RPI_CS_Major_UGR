#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char * name = argv[1];
    int fd = open(name, O_RDONLY);
    if (fd==-1){
        perror("open() failed");
        return EXIT_FAILURE;
    }
    char buffer[1024];
    int rc = read(fd,buffer,1023);
    buffer[rc] = '\0';
    char result[2];
    lseek(fd,0,SEEK_SET);
    for (int i=0;i<rc/7;i++){
        lseek(fd,6,SEEK_CUR);
        read(fd,result,1);
        result[1]='\0';
        printf("%s",result);
    }
    printf("\n");
    close(fd);
    return EXIT_SUCCESS;
}
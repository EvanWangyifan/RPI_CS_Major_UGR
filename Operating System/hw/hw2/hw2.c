#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>
#define SENTINEL_OUT "./sentinel.out"


// The function receive a file descripter and an array, it calculate all the
// counts and put them into the int array
void text_counter(int fd, int* result){
    int word_count=0, line_count=0, digit_count=0,read_outcome=1,char_count=0,status=0;
    char* letter = calloc(sizeof(char)+1,sizeof(char));
    while (read_outcome!=0){
        read_outcome = read(fd,letter,sizeof(char));
        if (read_outcome!=0){
            char_count++;
            if (isdigit(*letter)){
            digit_count++;
            }
            else if ((*letter)=='\n' || (*letter)=='\0'){
                line_count++;
            }
            if (!isalpha(*letter)){
                status=0;
            }
            else if (status==0){
                status=1;
                word_count++;
            }
        }
    }
    *(result)=word_count;
    *(result+1)=line_count;
    *(result+2)=digit_count;
    free(letter);
    return;
}

int length_helper(int x){
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

// Parent section
void parent(int n, char** file_names, int** p){ // Parent process
    int status;
    pid_t child_pid;
    int* data = calloc(4,sizeof(int));
    for (int i=0;i<n+1;i++){
        child_pid = waitpid(-1, &status, 0);   /* BLOCKING CALL */
        if ( WEXITSTATUS(status)==128 ){ // No output because of invalid file name
            //printf( "abnormally\n" );
            // PRINT NOTHING
        }
        else{ // Output results
            //int rc = WEXITSTATUS( status );
            //printf( "successfully with exit status %d\n", rc );
            close(*(*(p+i)+1));
            if (i==n){ // Sentinel
                int bytes_read = read(*(*(p+i)+0),data,4*sizeof(int));
                if ( bytes_read == -1 ){
                    perror( "read() failed on the pipe\n" );
                }
                else if ( bytes_read == 0 ){
                    perror( "PARENT: there are no write descriptors on the pipe\n" );
                    printf( "PARENT: child %d process must have terminated....\n",child_pid );
                }
                else{// Normal output
                    if (*(data+2)>1){
                        if (*(data+3)==1){
                            printf("PARENT: Sentinel -- %d bytes, %d words, %d lines, %d digit\n",*data,*(data+1),*(data+2),*(data+3));
                        }
                        else{
                            printf("PARENT: Sentinel -- %d bytes, %d words, %d lines, %d digits\n",*data,*(data+1),*(data+2),*(data+3));
                        }
                    }
                    else{
                        if (*(data+3)==1){
                            printf("PARENT: Sentinel -- %d bytes, %d words, %d lines, %d digit\n",*data,*(data+1),*(data+2),*(data+3));
                        }
                        else{
                            printf("PARENT: Sentinel -- %d bytes, %d words, %d lines, %d digits\n",*data,*(data+1),*(data+2),*(data+3));
                        }
                    }
                }
            }
            else{ // Normal
                int bytes_read = read(*(*(p+i)+0),data,4*sizeof(int));
                if ( bytes_read == -1 ){
                    perror( "read() failed on the pipe\n" );
                }
                else if ( bytes_read == 0 ){
                    perror( "PARENT: there are no write descriptors on the pipe\n" );
                    printf( "PARENT: child %d process must have terminated....\n",child_pid );
                }
                else{// Normal output
                    if (*(data+2)>1){
                        if (*(data+3)==1){
                            printf("PARENT: File \"%s\" -- %d bytes, %d words, %d lines, %d digit\n",*(file_names+i),*data,*(data+1),*(data+2),*(data+3));
                        }
                        else{
                            printf("PARENT: File \"%s\" -- %d bytes, %d words, %d lines, %d digits\n",*(file_names+i),*data,*(data+1),*(data+2),*(data+3));
                        }
                    }
                    else{
                        if (*(data+3)==1){
                            printf("PARENT: File \"%s\" -- %d bytes, %d words, %d line, %d digit\n",*(file_names+i),*data,*(data+1),*(data+2),*(data+3));
                        }
                        else{
                            printf("PARENT: File \"%s\" -- %d bytes, %d words, %d line, %d digits\n",*(file_names+i),*data,*(data+1),*(data+2),*(data+3));
                        }
                    }
                }
            }
            close(*(*(p+i)+0));
        }  
    }
    printf("PARENT: All done -- exiting...\n");
    free(data);
}


int main(int argc, char **argv){
    // Input checks
    if (argc<2){
        fprintf( stderr, "USAGE: <FileNames>...\n" );
        return EXIT_FAILURE;
    }
    int num_file = argc-1;
    // storing all filenames
    char** file_names = calloc(num_file,sizeof(char*));
    for (int i=0;i<num_file;i++){
        *(file_names+i) = *(argv+1+i);
    }

    // Initializations
    int** p = calloc(num_file+1,sizeof(int*));
    for (int i=0;i<num_file+1;i++){
        int* tmp = calloc(2,sizeof(int));
        *(p+i) = tmp;
        int pipe_rc = pipe( *(p+i) );
        if ( pipe_rc == -1 ){
            perror( "pipe() failed\n" );
            return EXIT_FAILURE;
        }
    }
    if (argc-1>1){
        printf("PARENT: Collecting counts for %d files and the sentinel...\n",argc-1);
    }
    else{
        printf("PARENT: Collecting counts for %d file and the sentinel...\n",argc-1);
    }
    for (int i=0;i<num_file;i++){
        printf("PARENT: Calling fork() to create child process for \"%s\"\n",*(argv+i+1));
    }
    printf("PARENT: Calling fork() to create child process for the sentinel\n");
    printf("CHILD: Calling execl() to execute sentinel.out...\n");
    fflush( stdout );
    // Start reading
    for (int i=0;i<num_file+1;i++){   
        pid_t pid = fork();
        
        if ( pid == -1 ){
            fprintf(stderr, "fork() failed");
            return EXIT_FAILURE;
        }
        // Child process
        if ( pid == 0 ){ 
            if (i==num_file){ // Sentinel.out situation
                close(*(*(p+i)+0));//stop reading
                int write_fd = *(*(p+i)+1);
                int length = length_helper( write_fd );
                char* pipe_write_fd = calloc(length+1,sizeof(char));
                sprintf(pipe_write_fd, "%d", write_fd);
                // printf("!!!%d\n",write_fd);
                *(pipe_write_fd+length) = '\0';
                // printf("!!!%s\n",pipe_write_fd);
                execl("./sentinel.out", "sentinel.out", pipe_write_fd, NULL);
                close(*(*(p+i)+1));
                free(pipe_write_fd);
                exit(i);
            }
            else{ // Normal situation
                printf("CHILD: Processing \"%s\"\n",*(argv+i+1));
                int* result = calloc(3,sizeof(int));
                int fd = open(*(file_names+i), O_RDONLY);
                if (fd==-1){
                    fprintf(stderr, "ERROR: stat() failed: No such file or directory\n");
                    printf("CHILD: Failed to process \"%s\"\n",*(argv+i+1));
                    exit(128);
                }
                struct stat st;
                stat(*(file_names+i),&st);
                int byte_count = st.st_size;
                text_counter(fd,result);

                close(*(*(p+i)+0));//stop reading
                int* d = calloc(4,sizeof(int));
                *d=byte_count;
                *(d+1)=*(result);
                *(d+2)=*(result+1);
                *(d+3)=*(result+2);
                write(*(*(p+i)+1),d,4*sizeof(int));
                close(*(*(p+i)+1));

                free(d);
                printf("CHILD: Done processing \"%s\"\n",*(argv+i+1));
                
                free(result);
                close(fd);
                exit(i);
            }
        }
    }
    parent(num_file,file_names,p);

    for (int i=0;i<num_file+1;i++){
        free(*(p+i));
    }
    free(p);
    free(file_names);
    
    return EXIT_SUCCESS;
}
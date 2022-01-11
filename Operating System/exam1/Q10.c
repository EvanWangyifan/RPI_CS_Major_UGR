#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>


int main(int argc, char **argv){
	setvbuf( stdout, NULL, _IONBF, 0 );
	int m = atoi(argv[1]);
	int p[2];
	char output[m];
	int pipe_rc = pipe(p);
	if ( pipe_rc == -1 ){
	    perror( "pipe() failed" );
	    return EXIT_FAILURE;
	}

	pid_t pid = fork();
    if ( pid == -1 ){
        fprintf(stderr, "fork() failed");
        return EXIT_FAILURE;
    }
    // Child process
    if ( pid == 0 ){
    	int status;
    	waitpid(getppid(),&status,0);
    	if (WIFSIGNALED(status)){
    		printf("terminate abnormally!!\n");
    		return EXIT_FAILURE;
    	}
    	int* byte = malloc(4);
    	int* digit = malloc(4);
    	*digit=0;

    	read(p[0],byte,sizeof(int));
    	read(p[0],output,sizeof(int));
    	char* line=malloc((*byte)*1);
    	int j=0;
    	for (int i=0;i<(*byte);i++){
    		if (isdigit(output[i])){
    			line[j]=output[i];
    			(*digit)++;
    			j++;
    		}
    	}
    	printf("%s\n(filtered %d digits from %d byte)\n",line,*digit,*byte);
    	free(byte);
    	free(digit);
    	free(line);
    }
    // Parent process
    else{
    	FILE* fd = fopen("data.txt", "r");
        int f_write = open("data.txt", O_RDONLY); 

        lseek (f_write, 1, SEEK_CUR);
    	int* count=malloc(1);
        struct stat st;
        stat("data.txt",&st);
        int byte_count = st.st_size;
        int a=1;
        if (a==0){
            printf("%d\n",byte_count);
        }
    	char c = fgetc(fd);
    	*count=0;
		while(!feof(fd)){
			if (*count>=m){
				break;
			}
			else{
				output[*count]=c;
				(*count)++;
			}
			c=fgetc(fd);
		}
		fclose(fd);
		write(p[1],count,sizeof(int));  
	    write(p[1],output,sizeof(int)); 
	    free(count);
    }



	return EXIT_SUCCESS;
}
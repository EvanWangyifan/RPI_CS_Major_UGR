#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/types.h>

#define MAX_SIZE_SHARED_MEMORY 32768

struct Q10{
    char * filename;
    int offset;
    char * data;
};

int counter=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void * trythis(void * tmp){
    struct Q10 d = *(struct Q10*)tmp;
    int fd = open(d.filename, O_RDONLY);
    lseek(fd, d.offset, SEEK_CUR);
    char* letter = calloc(1,1);
    int i=0;


    pthread_mutex_lock(&mutex);
    while(read(fd,letter,1)!=0){
        if (i==111){break;}
        if (isalnum(*letter)){
            *(d.data+counter)=*letter;
            i++;
            counter++;
        }else{
            continue;
        }
    }
    pthread_mutex_unlock(&mutex);
    

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int key = atoi(argv[1]);
    int id = shmget(key, MAX_SIZE_SHARED_MEMORY, IPC_CREAT | 0660);
    char * data = shmat(id, NULL, 0);
    
    char* filename = calloc(100,1);
    int index = 1;
    // read filename
    while (*(data+index)!='\0'){
        filename[index-1] = *(data+index);
        index++;
    }
    index++;
    // read N threads
    int num_children = *(int*) ((data+index));
    index++;
    index++;
    index++;
    index++;

    // read offsets
    int * offsets = calloc(num_children,4);
    for (int i=0;i<num_children;i++){
        offsets[i] = *(int*) ((data+index));
        index++;
        index++;
        index++;
        index++;
    }

    // create threads
    pthread_t tid[num_children];
    for (int i=0;i<num_children;i++){
        struct Q10* q = calloc(1,sizeof(struct Q10));
        q->filename = filename;
        q->offset = offsets[i];
        q->data = data+index;
        pthread_create(&tid[i],NULL,trythis,q);
    }

    // join threads
    int * retval;
    for (int i=0;i<num_children;i++){
        pthread_join(tid[i],(void**)&retval);
    }
    
    
    free(filename);
    free(offsets);
   

    data[0] = 'G';
	
	return EXIT_SUCCESS;
}
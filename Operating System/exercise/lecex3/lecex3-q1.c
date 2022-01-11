#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h> 
#include <fcntl.h>
#include <pthread.h>


int lecex3_q1_child( int pipefd ){
	int* k = (int*)malloc(sizeof(int));
	int* size = (int*)malloc(sizeof(int));
	read(pipefd, k, sizeof(int));
	read(pipefd, size, sizeof(int));

	int shmid = shmget(*k, *size, IPC_CREAT | 0660);
	char * data = shmat(shmid, NULL, 0);

	if(shmid == -1){
		perror("shmget() error");
		return EXIT_FAILURE;
	}

	if (data == (char*) -1){
		perror("shmget() error");
		return EXIT_FAILURE;
	}

	for(int i = 0; i<*(size); i++){
		data[i] = toupper(data[i]);
	}

	int rc = shmdt(data);

	if (rc == -1){
		perror("shmget() error");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;

}
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h> 
#include <fcntl.h>
#include <pthread.h>

void * copy_file(void * arg);

int main(int argc, char const *argv[1]){
	if (argc == 1) {
		printf("MAIN: Successfully copied 0 bytes via 0 child threads\n");
		return EXIT_SUCCESS;
	}
	pthread_t tid[argc-1];
	for (int i = 1; i < argc; i++){
		printf("MAIN: Creating thread to copy \"%s\"\n",argv[i]);
		int rc = pthread_create(&tid[i-1], NULL, copy_file, (void*)argv[i]);
		if(rc != 0){
			fprintf(stderr, "pthread_create() error (%d)\n",rc);
		}
	}
	int total = 0;
	for(int i = 1; i < argc; i++){
		int * x;
		pthread_join (tid[i-1], (void **) &x);
		total += *x;
		printf("MAIN: Thread completed copying %d bytes for \"%s\"\n", *x, argv[i]);
		free(x);
	}
	if(argc <= 2){
		printf("MAIN: Successfully copied %d bytes via %d child thread\n", total, argc - 1);
	}else{
		printf("MAIN: Successfully copied %d bytes via %d child threads\n", total, argc - 1);
	}
	return 0;

}

#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE * myfile;
	if (NULL == (myfile = fopen("lab03-data.dat","r+"))){
		perror("fopen failed in main");
		exit(-1);
	}
	unsigned long* result;
	result = (unsigned long*)calloc(93, sizeof(unsigned long));
	fread(result, 93, sizeof(unsigned long), myfile);
	fclose(myfile);
	int i;
	for(i=0; i<93; i++){
		printf("Data point # %d: %lu\n", i, result[i]);
	}
	free(result);
	return EXIT_SUCCESS;
}
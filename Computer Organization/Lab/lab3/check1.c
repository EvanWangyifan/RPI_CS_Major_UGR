#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE * myfile;
	if (NULL == (myfile = fopen("lab03-data.dat","r+"))){
		perror("fopen failed in main");
		exit(-1);
	}
	int* result;
	result = (int*)calloc(186, sizeof(int));
	fread(result, 186, sizeof(int), myfile);
	fclose(myfile);
	int i;
	for(i=0; i<186; i++){
		printf("Data point # %d: %d\n", i, result[i]);
	}
	free(result);
	return EXIT_SUCCESS;
}
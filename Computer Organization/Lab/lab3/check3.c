#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
	char line[128];
	FILE * myfile;
	if (NULL == (myfile = fopen("book-1984.txt","r+"))){
		perror("fopen failed in main");
		exit(-1);
	}
	while (fgets(line, 128, myfile)!=NULL){
		int counter = 0;
		char* ptr;
		for (ptr=line;(*ptr)!='\0';ptr++){
			if (isalnum(*ptr)){
				counter++;
				printf("%c",*ptr);
			}
		}
		if (counter!=0){
			printf(" [%d alnum chars]\n", counter);
		}
	}
	fclose(myfile);
	return EXIT_SUCCESS;
}
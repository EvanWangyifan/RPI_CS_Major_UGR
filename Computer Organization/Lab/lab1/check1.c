#include <stdio.h>
#include <string.h>
#include <math.h>

void print1(int n){
	for (int i=1; i<=n; i++){
		for (int j=1; j<=2*i-1; j++){
			printf("%c",'*');
		}
		printf("\n");
	}
}

void print2(int n){
	char a[20],b[20];
	strcpy(a,  "*");
   	strcpy(b, "**");
	for (int i=0; i<n; i++){
		printf("%s\n",a);
		strcat(a,b);
	}
}


int main(void){
	printf("What is n? ");
	int n;
	scanf("%u",&n);
	float x;
	x = sqrt(n*n+(2*n-1)*(2*n-1));
	

	printf("%.2f\n",x);

	print1(n);
	printf("\n");
	print2(n);


	return 0;
}
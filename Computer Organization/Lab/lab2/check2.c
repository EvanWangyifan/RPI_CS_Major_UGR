#include <stdio.h>
#include <stdlib.h>


int gcd(int a, int b){
	if (a==b) return a;
	if (a>b) return gcd(a-b,b);
	if (b>a) return gcd(b-a,a);
}

int main(){
	int a, b, result;
	printf("Enter the first number\n");
	scanf("%d",&a);
	printf("Enter the second number\n");
	scanf("%d",&b);
	result = gcd(a,b);
	printf("The greatest common divisor is %d\n", result);

	return EXIT_SUCCESS;
}
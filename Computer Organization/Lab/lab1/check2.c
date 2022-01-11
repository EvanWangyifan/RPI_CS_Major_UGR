#include <stdio.h>


int fib(long a){
	if (a==0 || a==1){
		return 0;
	}
	else if (a==2){
		return 1;
	}
	else{
		return (fib(a-1)+fib(a-2));
	}
}

int fib_itr(long n){
    long i;
    long f1 = 0;
    long f2 = 1;
    long fi;
 
    if(n == 0)
        return 0;
    if(n == 1)
        return 1;
 
    for(i = 2 ; i <= n ; i++ )
    {
        fi = f1 + f2;
        f1 = f2;
        f2 = fi;
    }
    return fi;
}

int main(void){
	long a, b, c;
	scanf("%ld",&a);
	b = fib(a);
	printf("%ld\n",b);
	c = fib(a);
	printf("%ld\n",c);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>

long fib(long a){
	if (a==0){
		return 0;
	}
	else if (a==2 || a==1){
		return 1;
	}
	else{
		return (fib(a-1)+fib(a-2));
	}
}

void fib_itr(long n, long* results){
    long i;
    long f1 = 0;
    long f2 = 1;
    long fi;
 
    if(n == 0){
    	results[0] = 0;
    }
    if(n == 1){
    	results[0]=0;
    	results[1]=1;
    }
 
    for(i = 2 ; i <= n ; i++ )
    {
        fi = f1 + f2;
        results[i]=fi;
        f1 = f2;
        f2 = fi;
    }
}



int main(int argc, char* argv[]){
	/*
	// method 1
	int i;
	for (i=1; i<argc; i++){
		long result;
		int num;
		num = atoi(argv[i]);
		result = fib(num);
		printf("fib(%d) is %ld\n",num,result);
	}
	*/
	
	// method 2
	int j;
	int max = 0;
	for (j=1; j<argc-1; j++){
		if (max<atoi(argv[j])){
			max = atoi(argv[j]);
		}
	}
	long* all_results;
	all_results = (long*)malloc(max*sizeof(long));


	fib_itr(max, all_results);

	printf("\n");
	int k;
	for (k=1; k<argc; k++){
		int num1;
		num1 = atoi(argv[k]);
		printf("fib(%d) is %ld\n",num1,all_results[num1]);
	}


	free(all_results);
	

	return EXIT_SUCCESS;
}
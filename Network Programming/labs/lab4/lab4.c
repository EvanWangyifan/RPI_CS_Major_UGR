#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct NUMS {
    int a;
    int b;
}NUMS;

int add(int a, int b) { 
    if(b > 0) 
        return 1 + add(a,b-1);
    else
        return a; 
}

void* child_cal (void* nums) {
    NUMS* nums_in = (NUMS*) nums;
    printf("Thread %ld running add() with [%d + %d]\n", pthread_self(), nums_in->a, nums_in->b);
    long result = (long) add(nums_in->a, nums_in->b);
    return (void*) result;
}

int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    int num_child = atoi(argv[1]);
    pthread_t children[num_child*(num_child-1)];
    NUMS* all_num[num_child*(num_child-1)];
    for(int i = 1; i < num_child; i++) {
        for(int j = 1; j <= num_child; j++) {
            int index = (i-1)*num_child + j-1;
            all_num[index] = calloc(1, sizeof(NUMS));
            all_num[index] -> a = i;
            all_num[index] -> b = j;
            pthread_t tid;
            int val = pthread_create(&tid, NULL, child_cal, (void*) all_num[index]);
            if(val < 0) {
                return -1;
            }
            else{
                children[index] = tid;
                printf("Main starting thread add() for [%d + %d]\n", i, j);
            }
            //printf("%d + %d\n", all_num[index] -> a, all_num[index] -> b);
        }
    }
    for(int i = 1; i < num_child; i++) {
        for(int j = 1; j <= num_child; j++) {
            int* ret;
            int index = (i-1)*num_child + j-1;
            pthread_join(children[index], (void**)&ret);
            printf("In main, collecting thread 140170751706880 computed [%d + %d] = %d\n", i, j, (int)ret);
            free(all_num[index]);
        }
    }
    return 0;
}
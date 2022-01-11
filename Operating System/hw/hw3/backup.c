#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// gcc -Wall -Werror -D NO_PARALLEL hw3-main.c hw3.c -pthread
// gcc -Wall -Werror hw3-main.c hw3.c -pthread

extern int next_thread_id;
extern int max_squares; // tracks the maximum number of squares covered by Sonny so far.
extern char *** dead_end_boards; // a list of dead end board
struct Knight* dead_end_knights; // a copy of dead end board list but in format of Knight object
int least_covered;
bool no_parallel = false;
int dead_end_boards_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // lock


struct Knight {
    char ** bd;
    int m;
    int n;
    int row;
    int col;
    int coverage;
    int max_coverage;
};



void bd_copy(char** new, char** old, int m, int n){
    for(int i = 0; i < m; i++) {
        new[i] = (char*) calloc(n, sizeof(char));
        strcpy(new[i], old[i]);
        //printf("check this: %s\n",new[i]);      // tag
    }
    return;
}

void * find_threads(void* arg){
    struct Knight * knight = arg;
    int thread_count = 0;
    int m = knight->m;
    int n = knight->n;
    int row = knight->row;
    int col = knight->col;
    //for(int i = 0; i < m; i++) {
        //printf("check this: %s\n",knight->bd[i]);      // tag
    //}

    // All possible new position for the knight
    int possible_X[8] = {row-2,row-2,row-1,row+1,row+2,row+2,row+1,row-1};
    int possible_Y[8] = {col-1,col+1,col+2,col+2,col+1,col-1,col-2,col-2};
    // find number of valid moves
    for (int i=0;i<8;i++){
        if(possible_X[i] == -100 || possible_Y[i] == -100){
            continue;
        }
        if (possible_X[i]<m && possible_Y[i]<n && 
                possible_X[i]>=0 && possible_Y[i]>=0 &&
                    knight->bd[possible_X[i]][possible_Y[i]]=='.'){
            thread_count++;
        }
        else{
            possible_X[i]=-100;
            possible_Y[i]=-100;
        }
    }

    int current_coverage, current_max = knight->max_coverage;
    // full tour or dead end
    if (thread_count==0){
        // CRITICAL OPERATIONS
        pthread_mutex_lock(&mutex);
        // update max coverage
        if (knight->coverage>max_squares){
            max_squares = knight->coverage;
        }
        knight->max_coverage = knight->coverage;
        // realloc for dead end board
        if (knight->coverage!=(m*n)){
            struct Knight tmp = {knight->bd,m,n,row,col,knight->coverage,knight->max_coverage};
            dead_end_knights = realloc(dead_end_knights, (dead_end_boards_count+1)*sizeof(struct Knight));
            dead_end_boards = realloc(dead_end_boards, (dead_end_boards_count+1)*sizeof(char*));
            dead_end_knights[dead_end_boards_count] = tmp;
            dead_end_boards[dead_end_boards_count] = knight->bd;
            dead_end_boards_count++;
        }
        pthread_mutex_unlock(&mutex);
        pthread_exit((void*)(intptr_t)knight->coverage);
        // Print out result
        if (knight->coverage==(m*n)){
            printf("THREAD %d: Sonny found a full knight's tour!\n", next_thread_id);
        }
        else{
            printf("THREAD %d: Dead end at move #%d\n",next_thread_id,knight->coverage);
        }
    }
    // just 1 possible move
    else if (thread_count==1){
        for (int i=0;i<8;i++){
            if (possible_X[i]!=-100 && possible_Y[i]!=-100 && knight->bd[possible_X[i]][possible_Y[i]]=='.'){
                knight->row = possible_X[i];
                knight->col = possible_Y[i];
                knight->bd[possible_X[i]][possible_Y[i]] = 'S';
                knight->coverage += 1;
                find_threads(knight); // recurse
                break;
            }
        }
    }
    // multiple possible move
    else if (thread_count>1){
        int counter = 0;
        int currentID = 1;
        pthread_t tid[thread_count];
        printf("MAIN: %d possible moves after move #%d; creating %d child threads...\n", thread_count,knight->coverage,thread_count);
        // create threads
        for (int i=0;i<8;i++){
            if (possible_X[i]!=-100 && possible_Y[i]!=-100){
                // create a new table
                char** tmp_bd = calloc(m,sizeof(char*));
                bd_copy(tmp_bd,knight->bd,m,n);
                tmp_bd[possible_X[i]][possible_Y[i]]='S';
                // 
                struct Knight tmp_knight = {tmp_bd,m,n,possible_X[i],possible_Y[i],knight->coverage+1,knight->max_coverage+1};
                int rc = pthread_create(&tid[counter],NULL,find_threads,&tmp_knight);
                if (rc != 0) {
                    fprintf(stderr, "Could not create thread (%d)\n", rc);
                }
                
                currentID++;
                pthread_mutex_lock(&mutex);
                next_thread_id++; // add 1 to the total thread
                pthread_mutex_unlock(&mutex);
                // no parallel mode
                if (no_parallel){
                    int rc1 = pthread_join(tid[counter],(void**)&current_coverage);
                    if (rc1 != 0) {
                        fprintf(stderr, "Could not create thread (%d)\n", rc);
                    }
                    
                    pthread_mutex_lock(&mutex);
                    if (current_coverage>current_max){
                        current_max = current_coverage;
                    }
                    knight->max_coverage = current_max;
                    pthread_mutex_unlock(&mutex);
                    printf("MAIN: Thread %d joined (returned %d)\n",currentID,current_coverage);
                }
                // free memory
                
                counter++;
                // for (int j=0;j<m;j++){
                //     free(tmp_bd[m]);
                // }
                // free(tmp_bd);
            }
        }
        
        // parallel
        if (!no_parallel){
            counter=0;
            for (int i=0;i<8;i++){
                if (possible_X[i]!=-100 && possible_Y[i]!=-100){
                    int rc2 = pthread_join( tid[counter], (void**)&current_coverage );
                    if(rc2 == 3) {
                        printf("MAIN: %d possible moves after move #%d; creating %d child threads...\n", thread_count,knight->coverage,thread_count);
                    }
                    pthread_mutex_lock(&mutex);
                    if (current_coverage>current_max){
                        current_max = current_coverage;
                    }
                    knight->max_coverage = current_max;
                    pthread_mutex_unlock(&mutex);
                    printf("MAIN: Thread %d joined (returned %d)\n",currentID,current_coverage);
                    counter++;
                }
            }
        }
    }
    current_max = knight->max_coverage;
    return EXIT_SUCCESS;
}



void print_bd(int m, int n, char** bd){
    for (int i = 0; i < m; i++) {
        if(i == 0) {
            printf("MAIN: >>");
        }
        else{
            printf("MAIN:   ");
        } 
        for (int j = 0; j < n; j++) {
            printf("%c", bd[i][j]);
        }
        if (i==m-1){
            printf("<<");
        }
        printf("\n");
    }
}

int simulate(int argc, char * argv[]){
    setvbuf(stdout, NULL, _IONBF, 0);
    if(argc != 5 && argc != 6){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }
    #ifdef NO_PARALLEL
    no_parallel = true;
    #endif
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int r = atoi(argv[3]);
    int c = atoi(argv[4]);
    int x = atoi(argv[5]);
    if(m < 2 || n < 2 || r>(m-1) || c>(n-1) || x<0 || x>(m*n)){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }
    least_covered = x;
    
    // initialize board with '.'
    char** init_bd = calloc(m, sizeof(char*));
    for (size_t i = 0; i < m; i++) {
        init_bd[i] = calloc(n, sizeof(char*));
    }
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            init_bd[i][j] = '.';
        }
    }
    init_bd[r][c] = 'S';
    dead_end_boards_count = 0;
    struct Knight start_knight = {init_bd, m, n, r, c, 1, 1};
    printf("MAIN: Solving Sonny's knight's tour problem for a %dx%d board\n", m, n);
    printf("MAIN: Sonny starts at row %d and column %d\n",r,c);

    // call recursive step
    find_threads(&start_knight);

    // get some final output
    if (max_squares<(m*n)){ // no full knight's tour
        printf("MAIN: All threads joined; best solution(s) visited %d squares out of %d\n",max_squares,m*n);
        printf("MAIN: Dead end boards covering at least %d squares are:",x);
        for (int i = 0; i < dead_end_boards_count; i++) {
            if(dead_end_knights[i].coverage >= least_covered){
                print_bd(m, n, dead_end_knights[i].bd);
            }
        }
    }
    else{ // full knight's tour achieved
        printf("MAIN: All threads joined; full knight's tour of %d achieved\n",m*n);
    }
    for (size_t i = 0; i < m; i++) {
        free(init_bd[i]);
    }
    free(init_bd);
    // free up memory
    // for (int i = 0; i < dead_end_boards_count-1; i++) {
    //    for (int j = 0; j < m; j++) {
    //         free(dead_end_knights[i].bd[j]);
    //         free(dead_end_boards[i][j]);
    //     }
    //     free(dead_end_knights[i].bd);
    // }
    free(dead_end_knights);
    return 0;
}

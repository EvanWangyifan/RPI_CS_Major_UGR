#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// gcc -Wall -Werror -D NO_PARALLEL hw3-main.c hw3.c -pthread
// gcc -Wall -Werror hw3-main.c hw3.c -pthread

#include <pthread.h>
#define MAX 8


struct Point
{
    int x;
    int y;
};

struct Board
{
    int r; 
    int c; 
    int current_coverage; 
    int max_coverage; 
    struct Point currentPoint; 
    int** data; 
};

// declare global variables
extern int next_thread_id;
extern int max_squares; // tracks the maximum number of squares covered by Sonny so far.
extern char *** dead_end_boards; 
int dead_ends_max_nums = 8; 
int dead_current_num = 0; 
bool noparallel = false;
struct Board* dead_end_boards_struct; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void* trythis(void* arg);

void printBoard(struct Board* b, int min){
    printf("MAIN: Dead end boards covering at least %d squares are:\n", min);
    for(int i = 0; i < dead_current_num; i++){
        if((b + i) -> current_coverage >= min){
            printf("MAIN: >>");
            for(int j = 0; j < (b + i) -> r; j++){
                if (j > 0){
                    printf("MAIN:   ");
                }
                for(int k = 0; k < (b + i) -> c; k++){
                    if (*(*((b + i) -> data + j) + k) > 0){
                        printf("S");
                    }else{
                        printf(".");
                    }
                }
                if (j==((b + i) -> r)-1){
                    printf("<<");
                }
                printf("\n");
            }
            
        }
    }
}

void copyBoard(struct Board* old, struct Board* new){
    new -> r = old -> r;
    new -> c = old -> c;
    new -> current_coverage = old -> current_coverage;
    new -> max_coverage = old -> max_coverage;
    new -> data = malloc(new -> r * sizeof(int*));
    for(int i = 0; i < new -> r; i++){
        *(new ->data + i) = malloc(new -> c * sizeof(int));
        for(int j = 0; j < new -> c; j++){
            *(*(new -> data + i) + j) = *(*(old -> data + i) + j);
        }
    }
}

int isValidPoint(struct Point point, struct Board board){
    if (point.x >= 0 && point.y >= 0 && point.x < board.r && point.y < board.c && *(*(board.data + point.x) + point.y) == 0){
        return true;
    }
    return false;
}

void createThreads(int numValid, struct Point* init_Points, struct Board* init_Board, int* number){
    int rc;
    pthread_t* pid = calloc(numValid,sizeof(pthread_t));

    printf("MAIN: %d moves possible after move #%d; creating %d child threads...\n", numValid, numValid, init_Board-> current_coverage);
    for(int i = 0; i < numValid; i++){ 
        struct Point p = init_Points[i];
        struct Board *b = calloc(1,sizeof(struct Board));
        copyBoard(init_Board, b);
        
        *(*(b -> data + p.x) + p.y) = b -> current_coverage + 1;
        b -> currentPoint = init_Points[i];
        b -> current_coverage = b -> current_coverage + 1;
        pthread_mutex_lock(&mutex);
        if (max_squares < b -> current_coverage){
            max_squares = b -> current_coverage;
        }
        pthread_mutex_unlock(&mutex);

        rc = pthread_create(&(*(pid + i)), NULL, trythis, (void*) b);
        next_thread_id++;
        if (rc != 0){
            fprintf(stderr, "ERROR: Could not create thread\n");
        }

        if (noparallel==1){
            int* x;
            rc = pthread_join(*(pid + i), (void**)& x);
            pthread_mutex_lock(&mutex);
            if (*x > *number){
                *number = *x;
            }
            pthread_mutex_unlock(&mutex);
            if (rc != 0){
                fprintf(stderr, "ERROR: No thread to join!\n");
            }else{
                printf("MAIN: Thread [%d] joined (returned %d)\n", next_thread_id, *x);         
            }
            free(x);
        }
    }

    if (noparallel == 0){
        for(int i = 0; i < numValid; i++){
            int* x;
            rc = pthread_join(*(pid + i), (void**)& x);
            pthread_mutex_lock(&mutex);
            if (*x > *number){
                *number = *x;
            }
            pthread_mutex_unlock(&mutex);
            if (rc != 0){
                fprintf(stderr, "ERROR: No thread to join!\n");
            }else{
                printf("MAIN: Thread [%d] joined (returned %d)\n", next_thread_id, *x);                
            }
            free(x);
        }
    }
    free(pid);
}

int ValidCounter(struct Point point, struct Board board, struct Point* validPoints){
    int numValid = 0;
    struct Point points[8];
    int counter=0;
    // evaluate the point clockwise
    struct Point p1;
    p1.x = point.x - 1;
    p1.y = point.y - 2;
    points[counter] = p1;
    counter++;

    struct Point p2;
    p2.x = point.x - 2;
    p2.y = point.y - 1;
    points[counter] = p2;
    counter++;

    struct Point p3;
    p3.x = point.x - 2;
    p3.y = point.y + 1;
    points[counter] = p3;
    counter++;

    struct Point p4;
    p4.x = point.x - 1;
    p4.y = point.y + 2;
    points[counter] = p4;
    counter++;

    struct Point p5;
    p5.x = point.x + 1;
    p5.y = point.y + 2;
    points[counter] = p5;
    counter++;

    struct Point p6;
    p6.x = point.x + 2;
    p6.y = point.y + 1;
    points[counter] = p6;
    counter++;

    struct Point p7;
    p7.x = point.x + 2;
    p7.y = point.y - 1;
    points[counter] = p7;
    counter++;
    
    struct Point p8;
    p8.x = point.x + 1;
    p8.y = point.y - 2;
    points[counter] = p8;
    
    for (int i=0;i<8;i++){
        if (isValidPoint(points[i], board)){
            *(validPoints + numValid) = points[i];
            numValid++;
        }
    }   
    return numValid;
}

void updateBoard(struct Board* b, struct Point* ValidPoints){
    *(*(b -> data + ValidPoints -> x) + ValidPoints -> y) = b -> current_coverage + 1;
    b -> currentPoint = *(ValidPoints);
    b -> current_coverage = b -> current_coverage + 1;
    if (max_squares < b -> current_coverage){
        pthread_mutex_lock(&mutex);
        max_squares = b -> current_coverage;
        pthread_mutex_unlock(&mutex);
    }
}

void addDeadBoard(struct Board* b){
    struct Board board;
    copyBoard(b, &board);
    pthread_mutex_lock(&mutex);
    if (dead_current_num >= dead_ends_max_nums){
        dead_end_boards = realloc(dead_end_boards, (dead_ends_max_nums +1) * sizeof(char*));
        dead_end_boards_struct = realloc(dead_end_boards_struct, (dead_ends_max_nums +1) * sizeof(struct Board));
        dead_ends_max_nums = dead_ends_max_nums+1;
    }
    dead_end_boards_struct[dead_current_num] = board;
    dead_current_num = dead_current_num + 1;
    pthread_mutex_unlock(&mutex);
}

void trythis_helper(struct Board* board, int* number){    
    struct Point* ValidPoints = calloc(MAX,sizeof(struct Point));
    int numValid = ValidCounter(board -> currentPoint, *board, ValidPoints);
    if (numValid==0){ // which means we really have multile way and we need to create thread
        if (board -> current_coverage < board -> max_coverage){
            addDeadBoard(board);
            printf("THREAD %d: Dead end after move #%d\n", next_thread_id, board -> current_coverage);
        }else{
            printf("THREAD %d: Sonny found a full knight's tour!\n", next_thread_id);
        }
    }else if (numValid==1){
        updateBoard(board, ValidPoints);
        pthread_mutex_lock(&mutex);
        *number = board -> current_coverage;
        pthread_mutex_unlock(&mutex);
        trythis_helper(board, number);
    }else if (numValid>1){
        createThreads(numValid, ValidPoints, board, number);
    }
    free(ValidPoints);
}

void* trythis(void* arg){
    int* x = calloc(1, sizeof(int));
    struct Board* b =  arg;
    *x = b -> current_coverage;
    struct Point* ValidPoints = calloc(MAX,sizeof(struct Point));
    int numValid = ValidCounter(b -> currentPoint, *b, ValidPoints);

    if (numValid==0){
        if (b -> current_coverage < b -> max_coverage){
            printf("THREAD %d: Dead end after move #%d\n", next_thread_id, b -> current_coverage);
            addDeadBoard(b);
        }else{
            printf("THREAD %d: Sonny found a full knight's tour!\n", next_thread_id);
        }
    }else if (numValid==1){
        updateBoard(b, ValidPoints);
        pthread_mutex_lock(&mutex);
        *x = b -> current_coverage;
        pthread_mutex_unlock(&mutex);
        trythis_helper(b, x);
    }else if (numValid > 1){
        createThreads(numValid, ValidPoints, b, x);
    }
    

    // free the memory
    free(ValidPoints);
    for(int i = 0; i < b -> r; i++){
        free(*(b -> data + i));
    }
    free(b -> data);
    free(b);
    
    pthread_exit(x);
}

int simulate(int argc, char * argv[]){
    #ifdef NO_PARALLEL
    noparallel = true;
    #endif
    setvbuf( stdout, NULL, _IONBF, 0 );
    dead_end_boards_struct = malloc(dead_ends_max_nums * sizeof(struct Board));
    if(argc != 5 && argc != 6){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }
    
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int r = atoi(argv[3]);
    int c = atoi(argv[4]);
    int x = atoi(argv[5]);
    if(m < 2 || n < 2 || r>(m-1) || c>(n-1) || x<0 || x>(m*n)){
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    

    // Start the program
    printf("MAIN: Solving Sonny's knight's tour problem for a %dx%d board\n", m, n);
    int** board = calloc(m, sizeof(int*));
    for(int i = 0; i < m; i++){
        *(board + i) = calloc(n, sizeof(int));
    }
    *(*(board)) = *(*(board)) + 1;
    
    struct Board init_Board;
    init_Board.r = m;
    init_Board.c = n;
    init_Board.data = board;
    init_Board.current_coverage = 1;
    init_Board.max_coverage = m*n;

    struct Point init_Point;
    init_Point.x = r;
    init_Point.y = c;
    init_Board.currentPoint = init_Point;
    
    struct Point* ValidPoints = calloc(MAX,sizeof(struct Point));
    int numValid = ValidCounter(init_Point, init_Board, ValidPoints);
    if (numValid > 1){ // create thread to run the program
        int *x = calloc(1, sizeof(int));
        createThreads(numValid, ValidPoints, &init_Board, x);
        free(x);
    }
    printf("MAIN: All threads joined; best solution(s) found visit %d squares out of %d\n", max_squares, m*n);
    if (dead_current_num!=0){
        printBoard(dead_end_boards_struct, m);
    }

    // free the memory
    for(int i = 0; i < dead_current_num; i++){
        for(int j = 0; j < (dead_end_boards_struct + i) -> r; j++){
            free(*((*(dead_end_boards_struct + i)).data + j));
        }
        free((dead_end_boards_struct + i) -> data);
    }
    free(dead_end_boards_struct);
    free(ValidPoints);
    for(int i = 0; i < m; i++){
        free(*(board + i));
    }
    free(board);
    return EXIT_SUCCESS;
}
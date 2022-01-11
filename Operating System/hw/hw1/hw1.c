#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int hash_function(int cache_size, char* word){
    int sum = 0;
    int i = 1;
    char letter = *word;
    while (letter!='\0'){
        sum += letter;
        letter = *(word+i);
        i++;
    }
    sum = sum%cache_size;
    return sum;
}


int main(int argc, char **argv){
    // Input checks
    setvbuf( stdout, NULL, _IONBF, 0 );
    if (argc < 3){
        fprintf( stderr, "USAGE: <CacheSize><FileName>\n" );
        return EXIT_FAILURE;
    }
    int cache_size = atoi(*(argv+1));
    if(cache_size == 0){
        fprintf( stderr, "ERROR: <Zero Cache Size>\n" );
        return EXIT_FAILURE;
    }
    int num_file = argc-2;
    char** file_names = calloc(num_file,sizeof(char*));
    for (int i=0;i<num_file;i++){
        *(file_names+i) = *(argv+2+i);
    }
    
    // Initializations
    char** cache = calloc(cache_size,sizeof(char*));
    char* word = calloc(128, sizeof(char));
    char* letter = calloc(sizeof(char)+1,sizeof(char));
    
    // Start reading
    for (int j=0;j<num_file;j++){
        int letter_count = 0; // Check if word length >= 3
        int read_outcome = 1; // Check if finish reading
        int fd = open(*(file_names+j), O_RDONLY);
        if (fd == -1){
            fprintf( stderr, "ERROR: <Open() Failed>\n" );
            return EXIT_FAILURE;
        }
        while (read_outcome!=0){
            read_outcome = read(fd,letter,sizeof(char));
            if (read_outcome!=0 && isalpha(*letter)){ // If normal words
                *(word+letter_count) = *letter; // add letter to word
                letter_count++;
            }
            else{ // If end of file or reach nondigit & nonalpha
                if (letter_count>2){
                    *(word+letter_count) = '\0';
                    int hash_i = hash_function(cache_size,word);
                    if (*(cache+hash_i)!=NULL){ // collision
                        *(cache+hash_i) = realloc(*(cache+hash_i),strlen(word)+1);
                        strcpy(*(cache+hash_i),word);
                        printf("Word \"%s\" ==> %d (realloc)\n", word, hash_i);
                    }
                    else{ // empty space
                        *(cache+hash_i) = calloc(strlen(word)+1,sizeof(char));
                        strcpy(*(cache+hash_i),word);
                        printf("Word \"%s\" ==> %d (calloc)\n", word, hash_i);
                    }
                }
                letter_count = 0;
            }
        }
        close(fd);
    }
    free(letter);
    free(word);
    free(file_names);
    // print cache table
    int i=0;
    while (i<cache_size){
        if (*(cache+i)!=NULL){
            printf("Cache index %d ==> \"%s\"\n", i, *(cache+i));
        }
        i++;
    }

    // deallocate cache table
    i=0;
    while (i<cache_size){
        if (*(cache+i)!=NULL){
            free(*(cache+i));
        }
        i++;
    }
    free(cache);
    


    return EXIT_SUCCESS;
}
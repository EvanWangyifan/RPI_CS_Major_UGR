#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char * reverse( char * s ){
    char * buffer = malloc(strlen(s)+1);
    int i, len = strlen( s );
    for ( i = 0 ; i < len ; i++ ) buffer[i] = s[len-i-1];
    buffer[i] = '\0';
    strcpy( s, buffer );
    free(buffer);
    return s;
}


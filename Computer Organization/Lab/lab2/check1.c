#include <stdio.h>
#include <stdlib.h>
int main() {
    /* Insert your four bytes of ASCII for your secret message */
    int z = 0x41424344;
      /* The 0x prefix above indicates a hexadecimal number */
    char * c = (char *)&z;
    printf( "%c", *c++ );
    printf( "%c", *c++ );
    printf( "%c", *c++ );
    printf( "%c\n", *c++ );
    return EXIT_SUCCESS;
  }

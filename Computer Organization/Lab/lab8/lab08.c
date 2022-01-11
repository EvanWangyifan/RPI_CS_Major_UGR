/* lab08.c */

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

/* define BIT type as a char (i.e., one byte) */ 
typedef char BIT;

/* function prototypes (basic gates) */
BIT not_gate( BIT );
BIT or_gate( BIT, BIT );
BIT and_gate( BIT, BIT );
BIT xor_gate( BIT, BIT );

/* function prototypes (combinational circuits) */
BIT multiplexer( BIT, BIT, BIT, BIT, BIT, BIT );
void decoder( BIT, BIT, BIT*, BIT*, BIT*, BIT* );

/* function prototypes (ALUs) */
BIT alu_1bit( BIT, BIT, BIT, BIT, BIT, BIT* );
void alu_4bit( BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT*, BIT*, BIT*, BIT*);

/* function prototype (floating point binary conversion) */
void ieee754encode( float value, char * encoded );

/* main() provides some unit testing */
int main()
{
   BIT A, B;
   BIT I0, I1, I2, I3;
   BIT O0, O1, O2, O3;
   BIT S0, S1;
   BIT OP0, OP1, CIN, COUT;
   BIT A0, A1, A2, A3, B0, B1, B2, B3, C0, C1, C2, C3, E0, E1, E2, E3;

   /* Unit test for NOT gate:
    *      _
    *  A | A
    * ---|---
    *  0 | 1
    *  1 | 0
    */
   printf( "===== Unit test for not_gate =====\n" );
   printf( "not_gate( A ) | RESULT\n" );
   printf( "--------------|--------\n" );
   printf( "not_gate( 0 ) | %d\n", not_gate( 0 ) );
   printf( "not_gate( 1 ) | %d\n", not_gate( 1 ) );
  

   /* Unit test for OR gate:
    *
    *  A  B | A OR B
    * ------|--------
    *  0  0 |   0
    *  0  1 |   1
    *  1  0 |   1
    *  1  1 |   1
    */
   printf( "\n===== Unit test for or_gate =====\n" );
   printf( "or_gate( A, B ) | RESULT\n" );
   printf( "----------------|--------\n" );

   for( A = 0 ; A < 2 ; A++ )
      for( B = 0 ; B < 2 ; B++ )
         printf( "or_gate( %d, %d ) | %d\n", A, B, or_gate( A, B ) );

   
   /* Unit test for AND gate:
    *
    *  A  B | A AND B
    * ------|--------
    *  0  0 |    0
    *  0  1 |    0
    *  1  0 |    0
    *  1  1 |    1
    */
   printf( "\n===== Unit test for and_gate =====\n" );
   printf( "and_gate( A, B ) | RESULT\n" );
   printf( "-----------------|--------\n" );

   for( A = 0 ; A < 2 ; A++ )
      for( B = 0 ; B < 2 ; B++ )
         printf( "and_gate( %d, %d ) | %d\n", A, B, and_gate( A, B ) );

   
   /* Unit test for XOR gate:
    *
    *  A  B | A XOR B
    * ------|--------
    *  0  0 |    0
    *  0  1 |    1
    *  1  0 |    1
    *  1  1 |    0
    */
   printf( "\n===== Unit test for xor_gate =====\n" );
   printf( "xor_gate( A, B ) | RESULT\n" );
   printf( "-----------------|--------\n" );

   for( A = 0 ; A < 2 ; A++ )
      for( B = 0 ; B < 2 ; B++ )
         printf( "xor_gate( %d, %d ) | %d\n", A, B, xor_gate( A, B ) );


   /* Unit test for 4-input multiplexer */
   printf( "\n===== Unit test for 4-input multiplexer =====\n" );
   printf( "multiplexer( S0, S1 ) | ( I0, I1, I2, I3 ) | RESULT ( I0 | I1 | I2 | I3 )\n" );
   printf( "----------------------|--------------------|------------------------------\n" );
   for( S0 = 0 ; S0 < 2 ; S0++ )
      for( S1 = 0 ; S1 < 2 ; S1++ )
         for( I0 = 0; I0 < 2; I0++ )
            for( I1 = 0; I1 < 2; I1++ )
               for( I2 = 0; I2 < 2; I2++ )
                  for( I3 = 0; I3 < 2; I3++ )
                     printf( "multiplexer( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) | %2d\n",
                             S0, S1, I0, I1, I2, I3, multiplexer( I0, I1, I2, I3, S0, S1 ) );


   /* Unit test for 2-input decoder:
    *
    *  I0 I1 | O0 O1 O2 O3
    * -------|-------------
    *   0  0 |  1  0  0  0
    *   0  1 |  0  0  1  0
    *   1  0 |  0  1  0  0
    *   1  1 |  0  0  0  1
    */
   printf( "\n===== Unit test for 2-input decoder =====\n" );
   printf( "decoder( I0, I1 ) | ( O0, O1, O2, O3 )\n" );
   printf( "------------------|--------------------\n" );
   /*
    * TO DO: Implement test cases
    */
    for (I0=0;I0<=1;I0++){
        for (I1=0;I1<=1;I1++){
            decoder(I0,I1,&O0,&O1,&O2,&O3);
            printf("decoder( %d, %d )   |   %d   %d   %d   %d\n", I0 , I1 , O0,O1,O2,O3);
        }
    }

   /* Unit test for 1-bit ALU:
    *  OP0 OP1 |  Operation
    * ---------|--------------
    *    0   0 |   A * B (AND)
    *    0   1 |   A + B (ADD)
    *    1   0 |   A + B (OR)
    *    1   1 |   A - B (SUB)
    *
    *  OP0 OP1 CIN  A  B | RESULT COUT
    * -------------------|-------------
    *    0   0   0  0  0 |      0    0
    *    0   0   0  0  1 |      0    0
    *    0   0   0  1  0 |      0    0
    *    0   0   0  1  1 |      1    1
    *    0   0   1  0  0 |      0    0
    *    0   0   1  0  1 |      0    1
    *    0   0   1  1  0 |      0    1
    *    0   0   1  1  1 |      1    1
    *    0   1   0  0  0 |      0    0
    *    0   1   0  0  1 |      1    0
    *    0   1   0  1  0 |      1    0
    *    0   1   0  1  1 |      0    1
    *    0   1   1  0  0 |      1    0
    *    0   1   1  0  1 |      0    1
    *    0   1   1  1  0 |      0    1
    *    0   1   1  1  1 |      1    1
    *    1   0   0  0  0 |      0    0
    *    1   0   0  0  1 |      1    0
    *    1   0   0  1  0 |      1    0
    *    1   0   0  1  1 |      1    1
    *    1   0   1  0  0 |      0    0
    *    1   0   1  0  1 |      1    1
    *    1   0   1  1  0 |      1    1
    *    1   0   1  1  1 |      1    1
    *    1   1   0  0  0 |      1    0
    *    1   1   0  0  1 |      0    0
    *    1   1   0  1  0 |      0    1
    *    1   1   0  1  1 |      1    0
    *    1   1   1  0  0 |      0    1
    *    1   1   1  0  1 |      1    0
    *    1   1   1  1  0 |      1    1
    *    1   1   1  1  1 |      0    1
    */
   printf( "\n===== Unit test for 1-bit alu =====\n" );
   printf( "alu_1bit( OP0, OP1, CIN, A, B ) | ( RESULT, COUT )\n" );
   printf( "--------------------------------|------------------\n" );
   /*
    * TO DO: Implement test cases
    */
    for(OP0 = 0; OP0 < 2; OP0++){
      for(OP1 = 0; OP1 < 2; OP1++){
        for(CIN = 0; CIN <2; CIN ++){
          for( A = 0 ; A < 2 ; A++ ){
            for( B = 0 ; B < 2 ; B++ ){
               BIT C = alu_1bit(OP0 , OP1, CIN, A, B, &COUT);
               printf("            %d   %d   %d   %d   %d   |      %d     %d \n", OP0 , OP1 ,CIN, A , B , C , COUT );
             }
          }
        }
      }
    }

   int failed = 0, total = 0, A4bit, B4bit, C4bit, expected = 0;
   printf( "\n===== Failed unit tests for 4-bit alu =====\n" );
   printf( "alu_4bit( OP0, OP1, A0, A1, A2, A3, B0, B1, B2, B3 ) | ( C0, C1, C2, C3 ) | expected ( E0, E1, E2, E3 )\n" );
   printf( "-----------------------------------------------------|--------------------|-----------------------------\n" );
   /*
    * TO DO: Implement test cases. Verify by comparing with the "expected" 
    * values and only print a message if there is a mismatch as shown in the example:
    */
     
    for (OP0 = 0; OP0 < 2; OP0 ++){
     for (OP1 = 0; OP1 < 2; OP1 ++){
      for (A0 = 0; A0 < 2; A0 ++){
       for (A1 = 0; A1 < 2; A1 ++){
        for (A2 = 0; A2 < 2; A2 ++){
         for (A3 = 0; A3 < 2; A3 ++){
          for (B0 = 0; B0 < 2; B0 ++){
           for (B1 = 0; B1 < 2; B1 ++){
            for (B2 = 0; B2 < 2; B2 ++){
             for (B3 = 0; B3 < 2; B3 ++){
              total++;
              if ( !A3 ) A4bit = 0;
              else A4bit = ~0x7;
              A4bit |= A2 << 2;
              A4bit |= A1 << 1;
              A4bit |= A0;
              if ( !B3 ) B4bit = 0;
              else B4bit = ~0x7;
              B4bit |= B2 << 2;
              B4bit |= B1 << 1;
              B4bit |= B0;
              alu_4bit( OP0, OP1, A0, A1, A2, A3, B0, B1, B2, B3, &C0, &C1, &C2, &C3 );
              if ( !C3 ) C4bit = 0;
              else C4bit = ~0x7;
              C4bit |= C2 << 2;
              C4bit |= C1 << 1;
              C4bit |= C0;
              if ( !OP0 && OP1 ) expected = A4bit + B4bit;
              else if (OP0 && !OP1) expected = A4bit | B4bit;
              else if (OP0 && OP1) expected = A4bit - B4bit;
              else if (!OP0 && !OP1) expected = A4bit & B4bit;
              E0 = expected & 0x01;
              E1 = ( expected & 0x02 ) >> 1;
              E2 = ( expected & 0x04 ) >> 2;
              E3 = ( expected & 0x08 ) >> 3;
              if ( ( expected & 0xf ) != ( C4bit & 0xf ) ) {
                  printf( "alu_4bit( %3d, %3d, %2d, %2d, %2d, %2d, %2d, %2d, %2d, %2d ) "
                          "| ( %2d, %2d, %2d, %2d ) | expected ( %2d, %2d, %2d, %2d )\n",
                          OP0, OP1, A0, A1, A2, A3, B0, B1, B2, B3, C0, C1, C2, C3, E0, E1, E2, E3 );
                  failed++;
              }
           }
          }
         }
        }
       }
      }
     }
    }
     }
    }
    /*
    * Make sure you test all ALU operations on all possible combinations of input values.
    * Print statistics of failed and total test cases in the end.
    */
   
   printf("%d test cases out of %d failed.\n", failed, total);

   return EXIT_SUCCESS;
}


BIT not_gate( BIT A )
{
   /* TO DO: implement logical NOT */
    if (A==1){
        return 0;
    }
    return 1;
}


BIT or_gate( BIT A, BIT B )
{
   /* TO DO: implement logical OR */
    if(A == 1) return 1;
    if(B == 1) return 1;
    return 0;
}


BIT and_gate( BIT A, BIT B )
{
   /* TO DO: implement logical AND */
    if (A==1 && B==1){
        return 1;
    }
    return 0;
}


BIT xor_gate( BIT A, BIT B )
{
   /* TO DO: implement logical XOR */
    if (A!=B){
        return 1;
    }
   return 0;
}


BIT multiplexer( BIT I0, BIT I1, BIT I2, BIT I3, BIT S0, BIT S1 )
{
   /* TO DO: implement a 4-input multiplexer */
    if (S0==0 && S1==0){
        return I0;
    }
    else if (S0==0 && S1==1){
        return I1;
    }
    else if (S0==1 && S1==0){
        return I2;
    }
    else if (S0==1 && S1==1){
        return I3;
    }
    return 0;
}


void decoder( BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3 )
{
   /* TO DO: implement a 2-input decoder */
    if (I0==0 && I1==0){
        *O0 = 1;
    }
    else if (I0==0 && I1==1){
        *O0=0;
        *O1 = 1;
    }
    else if (I0==1 && I1==0){
        *O0=0;
        *O1=0;
        *O2 = 1;
    }
    else if (I0==1 && I1==1){
        *O0=0;
        *O1=0;
        *O2=0;
        *O3 = 1;
    }
    return;
}


BIT alu_1bit( BIT OP0, BIT OP1, BIT CIN, BIT A, BIT B, BIT *COUT )
{
   /* TO DO: implement a 1-bit ALU */
    *COUT = (A & B) | ((A^B)&CIN);
    if(!OP0  && !OP1){
        return A&&B;
    }
    else if(!OP0 && OP1){
        return A^B^CIN;
    }
    else if(OP0 &&!OP1){
        return A||B;
    }
    else{
        B = !B;
        *COUT = (A & B) | ((A^B)&CIN);
        return A^B^CIN;
    }
}


void alu_4bit( BIT OP0, BIT OP1, BIT A0, BIT A1, BIT A2, BIT A3, 
              BIT B0, BIT B1, BIT B2, BIT B3,
              BIT *C0, BIT *C1, BIT *C2, BIT *C3 )
{
   /* TO DO: implement a 4-bit ALU */
    BIT COUT;
    if(OP1 && OP0)
    {
      *C0 = alu_1bit(OP0, OP1, 1, A0, B0, &COUT);
    }
    else
    {*C0 = alu_1bit(OP0, OP1, 0, A0, B0, &COUT);

    }
    *C1 = alu_1bit(OP0, OP1, COUT, A1, B1, &COUT);
    *C2 = alu_1bit(OP0, OP1,  COUT, A2, B2, &COUT);
    *C3 = alu_1bit(OP0, OP1, COUT, A3, B3, &COUT);
}

void ieee754encode( float value, char * encoded )
{
     /* TO DO: implement a function to encode a floating-point value
      * in the IEEE 754-1985 form
      */
    return;
}

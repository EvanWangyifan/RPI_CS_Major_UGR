/* lab08.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define UNDEF -1
#define FALSE 0
#define TRUE 1

/* define BIT type as a char (i.e., one byte) */
typedef char BIT;

void ieee754encode( float value, char * encoded );
/* main() provides some unit testing */
int main()
{
   BIT A, B;
   BIT I0, I1, I2, I3;
   BIT O0, O1, O2, O3;
   BIT S0, S1;
   BIT S, R, Q;

   /* Unit test for NOT gate:
    *      _
    *  A | A
    * ---|---
    *  0 | 1
    *  1 | 0
    */

   float value = 57.75;
   char* encoded = NULL;

   ieee754encode(value,encoded);

   return EXIT_SUCCESS;
}

int get_first_1(int* array , int len){
  int n=0;
  for(;n<len;n++){
    if(array[n]==1){
      return n;
    }
  }
}

void ieee754encode( float value, char * encoded ){
  int int_part = (int) value;
  float point_part = value - int_part;

  int* int_array = (int*)malloc(sizeof(int)*33);
  int* dec_array = (int*)malloc(sizeof(int)*33);
  int * fraction_integer;
  int cnt =0;  int pos = 0;
  int cnt_int;
  int tmp;
  int expo;
  printf("input: %.5f\n",value);
  encoded = (char*) malloc(sizeof(char)*33);
  printf("sign :");
  if(value <0){
    value = -value;
    encoded[0] = 1;
    printf("%d\n",encoded[0]);
  }
  else{
    encoded[0] = 0;
    printf("%d\n",encoded[0]);
  }
  while(int_part !=0){
    //printf("int_part is %d and times should be %d and the left %d\n",int_part, int_part/2,int_part-(int_part/2)*2 );
    int times = int_part/2;
    int left = int_part - 2*times;
    int_part =times;
    cnt++;
    int_array[pos] = left;
    pos++;
  }
  cnt_int = pos;
  fraction_integer = (int*) malloc(sizeof(int)*cnt);
  for(tmp =0 ; tmp < cnt; tmp++){
    fraction_integer[cnt-tmp-1] = int_array[tmp];
    //printf("%d ",fraction_integer[cnt-tmp]);
  }
  cnt =0 ;
  pos =8 ;
  //printf("%f\n",point_part);
  while(point_part !=0){
    //printf("%f ",point_part);
    point_part = point_part*2;
    if(point_part>=1){
      dec_array[cnt] = 1;
      cnt++;
      point_part = point_part-1;
    }
    else{
      dec_array[cnt] =0;
      cnt++;
    }
    pos--;
  }
  //printf("\n");
  //printf("%d\n",cnt);
  //printf("the cnt int is %d\n",cnt_int);
  int pos_int = get_first_1(fraction_integer,cnt_int);
  int pos_dec = get_first_1(dec_array,cnt);
  if(cnt_int!=0){
    expo = cnt_int -pos_int-1;
  }
  else{
    expo = pos_dec+1;
  }
  expo = expo +127;
  //printf("the expo is %d \n",expo);
  int* exponent = (int*) malloc(sizeof(int)*8);
  if(expo %2){
    exponent[0] = 1;
    expo=expo -1;
  }
  //printf("the log 2 is %d\n", (int)log2(6));
  for(tmp=1 ; tmp<8; tmp++){
    exponent[tmp] = 0;
  }
  while(expo != 0){
    exponent[(int)log2(expo)] =1;
    //printf("%d ",exponent[(int)log2(expo)]);
    expo = expo - pow(2, (int)log2(expo));
  }
  int loc= 1;
  printf("exponent:  ");
  for(tmp=0; tmp<8;tmp++){
    encoded[tmp+1] = exponent[7-tmp];
    loc++;
    printf("%d ",exponent[7-tmp]);
  }
  printf("\nfraction: ");
  for(tmp =1 ; tmp<cnt_int;tmp++){
    encoded[loc] = fraction_integer[tmp];
    loc++;
    printf("%d ",fraction_integer[tmp]);
  }
  for(tmp = 0; tmp< cnt; tmp++){
    encoded[loc] = dec_array[tmp];
    loc++;
    printf("%d ",dec_array[tmp]);
  }
  for(tmp=0;tmp< 24-cnt_int-cnt; tmp++){
    encoded[loc] =0;
    loc++;
    printf("%d ", 0);
  }
  printf("loc is now %d",loc);
  printf("\noutput:   ");
  for(tmp=0; tmp<32;tmp++){
    printf("%d",encoded[tmp]);
  }
  printf("\nend\n");

  return;
}

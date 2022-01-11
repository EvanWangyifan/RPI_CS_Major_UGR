/* adder.c */

#include <stdio.h>
#include <stdlib.h>

int add_two_bits(int d0, int d1, int carry_in, int *carry_out)
{
    int sum = d0 + d1 + carry_in;
    if (sum >= 2)
    {
        sum-=2;
        *carry_out = 1;
    }
    return sum;
}

int main()
{
    int i;
    int d0[4] = {1,1,1,1};
    int d1[4] = {1,1,1,1};
    int ci[4] = {0, 0, 0, 0};
    int sum[4] = {0, 0, 0, 0};
    int carry_out;
    /* Call add_two_bits() multiple times to implement a four-bit adder: */
    int start = 1;
    for (int i = 3; i>=0; i--)
    {
        carry_out = 0;
        if (start)
        {
            start = 0;
            sum[i] = add_two_bits(d0[i], d1[i], 0, &carry_out);
            if (carry_out) ci[i] = carry_out;
        }
        else{
            sum[i] = add_two_bits(d0[i], d1[i], ci[i+1], &carry_out);
            if (carry_out) ci[i] = carry_out;
        }
    }
    for (int i = 0; i< 4; i++)
    {
        printf("%d",sum[i]);
    }
    
    
    
    
    


    return EXIT_SUCCESS;
}

/* hw2.c */
/* NAME: <Yifan Wang> */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main( int argc, char * argv[] )
{
  char line[256];

  printf("Please enter a valid C assignment statement:\n");
  if (fgets(line, 256, stdin) == NULL)
  {
    fprintf(stderr, "fgets() failed");
    return EXIT_FAILURE;
  }

  printf("The MIPS pseudocode is:\n");

  // Do your magic here
  char all_var_c[100]; // store the c var that appears
  int all_var_mips[100]; // store the mips var's int (0 for s0)
  int array_index = 0; // how many vars already stored in these two array
  int sign_index = 6; // to read sign from line
  int var_index1 = 4; // read c var1 from line
  int var_index2 = 8; // read c var2 from line

  // Store the LHS variable
  all_var_c[array_index] = line[0];
  all_var_mips[array_index] = 0;
  array_index++;
  all_var_c[array_index] = line[4];
  all_var_mips[array_index] = 1;
  array_index++;

  // big loop reading every operation and output mips commands
  int temp_index = 0;
  while (line[var_index1+1]!=';'){
  	printf("hdbvnjxdbk%c\n", line[var_index1+1]);
  	// Determine the RHS variables
  	char var[10]; // for usage if the int is longer
  	
  	// read the line and store the vars into the array
  	if (islower(line[var_index2])){
  		//printf("%c\n", line[var_index2]);
  		var[0] = line[var_index2];
  		all_var_c[array_index] = line[var_index2];
  		//printf("%c", all_var_c[array_index]);
  		all_var_mips[array_index] = array_index;
  		array_index++;
  		//printf("%d\n",array_index);
  	}
  	// Store the int
  	else if (isdigit(line[var_index2])){
  		//printf("%d\n", var_index2);
  		int i, a=0;
  		if (line[sign_index]=='-'){
  			var[0] = '-';
  			a++;
  		}
  		i=a;
  		while(1)
  		{
  			//printf("line[var_index2]%c\n",line[var_index2]);
  			//printf("%c\n",line[var_index2]);
  			if (isdigit(line[var_index2]) )
  			{	

  				//printf("line[var_index2]%c\n",line[var_index2]);
  				var[i] = line[var_index2];
  				var_index2++;
  				//printf("var_index2%d\n",var_index2);
  				i++;
  			}
  			else
  			{ 
          printf("!!!!!!!");
          var_index2--;
  				//printf("%c\n",line[var_index2]);
  				break;
  			}
  			
  		}
  		var[i+1] = '\0';
  	}

  	temp_index++;
  	//printf("#####%d\n",var_index2);
  	// get to next sign and var
  	int i;

  	var_index1 = var_index2;
  	for (i=0;i<4;i++){
  		sign_index++;
  		var_index2++;
  	}
  }
  return EXIT_SUCCESS;
}

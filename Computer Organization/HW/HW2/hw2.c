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
  int array_index = 0; // how many vars already stored in these two array
  int sign_index = 6; // to read sign from line
  int var_index1 = 4; // read c var1 from line
  int var_index2 = 8; // read c var2 from line
  int first_element_digit = 0;// see if the first RHS element is digit
  int flag_for_first_line = 0;// see if is printing the first line
  
  // the LHS is not length 1 letter otherwise report error
  if (line[1]!=' ' || !islower(line[0])){
  	fprintf( stderr, "Parsing error...\n" ); 
  	return EXIT_FAILURE;
  }


  // If the first element on the right is int
  if (isdigit(line[var_index1])){
  	first_element_digit = 1;
  	char var[10]; // for usage if the int is longer
  	int i, a=0;
  	flag_for_first_line++;
	if (line[sign_index]=='-'){
		var[0] = '-';
		a++;
	}
	// Store the int and move all the index
	i=a;
	while(1){
		if (isdigit(line[var_index1]) )
		{	
			var[i] = line[var_index1];
			sign_index++;
			var_index1++;
			var_index2++;
			i++;
		}
		else
		{	
			sign_index--;
			var_index1--;
			var_index2--;
			break;
		}
		
	}
	var[i] = '\0';
	printf("addi $t0,$zero,%s\n",var);
  }

  // Store the LHS variable
  all_var_c[array_index] = line[0];
  array_index++;
  // Store the first element on RHS if it's not int
  if (first_element_digit==0){
  	all_var_c[array_index] = line[4];
  	array_index++;
  }
  // big loop reading every operation and output mips commands
  int temp_index = 0;
  if (first_element_digit==1){
  	temp_index++;
  }
  while (line[var_index1+1]!=';'){
  	// Determine the RHS variables
  	char var[10]; // for usage if the int is longer
  	int temp_sign_index = sign_index;
  	// read the line and store the vars into the array
  	if (islower(line[var_index2])){
  		var[0] = line[var_index2];
  		int flag = 0;// flag to see if already stored
  		int i;
  		// See if already in
  		for (i=0;i<=array_index;i++){
  			if (all_var_c[i]==line[var_index2]){
  				flag = 1;
  			}
  		}
  		// Not in, store it
  		if (flag == 0){
  			all_var_c[array_index] = line[var_index2];
  			array_index++;
  		}
  	}
  	// Store the int
  	else if (isdigit(line[var_index2])){
  		int i, a=0;
  		if (line[sign_index]=='-'){
  			var[0] = '-';
  			a++;
  		}
  		// move all index
  		i=a;
  		while(1){
  			if (isdigit(line[var_index2]) )
  			{	
  				var[i] = line[var_index2];
  				sign_index++;
  				var_index2++;
  				i++;
  			}
  			else
  			{	
  				sign_index--;
  				var_index2--;
  				break;
  			}
  			
  		}
  		var[i] = '\0';
  	}

  	
  	// Determine plus or minus
  	if (line[temp_sign_index]=='+' || 
  		(line[temp_sign_index]=='-' && isdigit(line[temp_sign_index+2]))){
  		printf("add");
  		// Decide if plus a int
  		if(isdigit(line[temp_sign_index+2])){
  			printf("i");
  		}
  		else{
  			printf(" ");
  		}
  		printf(" ");
  	}
  	else if (line[temp_sign_index]=='-'){
  		printf("sub  ");
  	}
  	
  	//If is the last line, use s0, otherwise t for the first address
  	if (line[var_index2+1] == ';'){
  		printf("$s0,");
  	}
  	else{
  		printf("$t%d,", temp_index);
  	}

  	// second address
  	if (temp_index==0 && flag_for_first_line==0){
  		flag_for_first_line++;
  		printf("$s1,");
  	}
  	else{
  		if (temp_index==0){
  			printf("$t9,");
  		}
  		else{
  			printf("$t%d,", temp_index-1);
  		}
  	}
	
  	// third address
  	// If add int print out the int
  	if (isdigit(var[0]) || var[0]=='-'){
  		printf("%s\n",var);
  	}
  	// If it's a var
  	else{
  		int i;
  		for (i=0;i<array_index;i++){
  			if (all_var_c[i]==var[0]){
  				printf("$s%d\n",i);
  				break;
  			}
  		}
  	}

  	temp_index++;
  	// Reuse temp if it reaches 9
  	if (temp_index==10){
  		temp_index = 0;
  	}
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

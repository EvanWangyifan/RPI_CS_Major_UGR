#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]){

	// Read the file and store it in 2D array
	FILE *file = fopen(argv[1],"r");
	char line[20][256];
	int line_num = 0;
	while (fgets(line[line_num], 256, file)){
		line_num++;
	}
	//strcat(line[line_num-1],"\n");
	fclose(file);

	char all_var_c[100]; // store the c var that appears
	int array_index = 0; // how many vars already stored
	int s_index = 0; // index of s register
	int temp_index = 0; // index of t register
	int line_index = 0; // index of line
	int t_used = 0; // 1 if t already used
	int L_index = 0; // index of L label
	while (line_index<line_num){ // while the line is not empty
		// global vars
		printf("# ");
		printf("%s",line[line_index]);
	  	int sign_index = 6; // to read sign from line
	  	int var_index1 = 4; // read c var1 from line
	  	int var_index2 = 8; // read c var2 from line
	  	int flag_for_first_line = 0;// see if is printing the first line

	  	// If the line is a simple assignment
	  	if (isdigit(line[line_index][var_index1]) || line[line_index][var_index1]=='-'){
	  		char var[10]; // int
	  		int i=0;
	  		if (line[line_index][var_index1]=='-'){
	  			var[0] = '-';
	  			i++;
	  			var_index1++;
	  		}
	  		// move all index
	  		while(1){
	  			if (isdigit(line[line_index][var_index1]) ){	
	  				var[i] = line[line_index][var_index1];
	  				var_index1++;
	  				i++;
	  			}
	  			else{	
	  				break;
	  			}
	  		}
	  		var[i] = '\0';
	  		all_var_c[array_index] = line[line_index][0]; // store the variable name
	  		array_index++;
	  		printf("li $s%d,%s\n",s_index,var);
	  		s_index++;
	  	}

	  	// Not an assignment
	  	else{
	  		all_var_c[array_index] = line[line_index][0]; // store the lhs var
	  		array_index++;


		  	// Big loop go through each row
		  	while (line[line_index][var_index1+1]!=';'){

		  		// Determine the RHS variables
			  	char var[10]; // for usage if the int is longer
			  	int temp_sign_index = sign_index;
			  	// read the line and store the vars into the array
			  	//printf("%c\n",line[line_index][var_index2]);
			  	if (islower(line[line_index][var_index2])){
			  		var[0] = line[line_index][var_index2];
			  		var[1] = '\0';
			  		int flag = 0;// flag to see if already stored
			  		int i;
			  		// See if already in
			  		for (i=0;i<=array_index;i++){
			  			if (all_var_c[i]==line[line_index][var_index2]){
			  				flag = 1;
			  			}
			  		}
			  		// Not in, store it
			  		if (flag == 0){
			  			all_var_c[array_index] = line[line_index][var_index2];
			  			array_index++;
			  		}
			  	}
			  	// Store the int
			  	else if (isdigit(line[line_index][var_index2]) 
			  			|| line[line_index][var_index2]=='-'){
			  		int i=0;
			  		if (line[line_index][var_index2]=='-' || line[line_index][sign_index]=='-'){
			  			var[0] = '-';
			  			i++;
			  			if (line[line_index][var_index2]=='-'){
			  				sign_index++;
			  				var_index2++;
			  			}
			  		}
			  		// move all index
			  		while(1){
			  			if (isdigit(line[line_index][var_index2]) )
			  			{	
			  				var[i] = line[line_index][var_index2];
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
			  			var[i] = '\0';
			  		}
			  	}

			  	// If addition and subtraction
			  	if (line[line_index][temp_sign_index]=='+' 
			  		|| line[line_index][temp_sign_index]=='-'){
				  	// Determine plus or minus
				  	if (line[line_index][temp_sign_index]=='+' || 
				  		(line[line_index][temp_sign_index]=='-' 
				  			&& isdigit(line[line_index][temp_sign_index+2]))){
				  		printf("add");
				  		// Decide if plus a int
				  		if(isdigit(line[line_index][temp_sign_index+2])){
				  			printf("i");
				  		}
				  		printf(" ");
				  	}
				  	else if (line[line_index][temp_sign_index]=='-'){
				  		printf("sub ");
				  	}

				  	// FIRST ADDRESS
				  	// If is the last line, use s
				  	if (line[line_index][var_index2+1] == ';'){
				  		printf("$s%d,",s_index);
				  		s_index++;
				  	}
				  	// otherwise t for the first address
				  	else{
				  		printf("$t%d,", temp_index%10);
				  		t_used = 1;
				  	}
				  	// SECOND ADDRESS
				  	if (temp_index==0 && flag_for_first_line==0){
				  		flag_for_first_line++;
				  		printf("$s0,");
				  	}
				  	
				  	printf("$t%d,", temp_index%10);
				  	
  					// THIRD ADDRESS
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
				}
				// If it is multiplication or div
				else if (line[line_index][temp_sign_index]=='*' 
					|| line[line_index][temp_sign_index]=='/'
					|| line[line_index][temp_sign_index]=='%'){
					
					// If just mult and div var
					if (islower(line[line_index][var_index2])){
						// If mult
						if (line[line_index][temp_sign_index]=='*' 
							|| line[line_index][temp_sign_index]=='/'
								|| line[line_index][temp_sign_index]=='%'){
							if (line[line_index][temp_sign_index]=='*'){
								printf("mult ");
							}
							else{
								printf("div ");
							}
							// mult
							// FIRST ADDRESS
							int i;
							if (flag_for_first_line==0){ // First line use s
					  			for (i=0;i<array_index;i++){
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$s%d,",i);
					  					break;
					  				}
					  			}
					  			flag_for_first_line++;
					  		}
					  		else{ // not first line use t
					  			printf("$t%d,",(temp_index-1)%10);
					  		}
					  		// SECOND ADDRESS
					  		for (i=0;i<array_index;i++){
					  			if (all_var_c[i]==line[line_index][var_index2]){
					  				printf("$s%d\n",i);
					  				break;
					  			}
					  		}

					  		// mflo
					  		if (line[line_index][temp_sign_index]!='%'){
					  			printf("mflo ");
					  		}
					  		else{
					  			printf("mfhi ");
					  		}
					  		if (line[line_index][var_index2+1]==';'){ // Use s if last address
					  			for (i=0;i<array_index;i++){
						  			if (all_var_c[i]==line[line_index][0]){
						  				printf("$s%d\n",i);
						  				break;
						  			}
					  			}
					  		}
					  		else{ // use t
					  			printf("$t%d\n",temp_index%10);
					  			t_used = 1;
					  		}
						}
					}
					else{ // If mult or sub int
						// Constant Multiplication
						if (line[line_index][temp_sign_index]=='*'){
							int i;
							// if mult 0
							if (var[0]=='0'){
								printf("li ");
								for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][0]){
					  					printf("$s%d,",i);
					  					break;
					  				}
					  			}
								printf("0\n");
							}
							else if (var[0]=='1'){ // mult 1
								printf("move $t%d,",temp_index%10);
								t_used = 1;
								for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$s%d\n",i);
					  					break;
					  				}
					  			}
					  			printf("move ");
					  			for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][0]){
					  					printf("$s%d,",i);
					  					break;
					  				}
					  			}
					  			printf("$t%d\n",temp_index%10);
							}
							else if (var[0]=='-' && var[1]=='1' && !isdigit(var[2])){ // mult -1
								printf("move $t%d,",temp_index%10);
								t_used = 1;
								for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$s%d\n",i);
					  					break;
					  				}
					  			}
					  			printf("sub ");
					  			for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][0]){
					  					printf("$s%d,",i);
					  					break;
					  				}
					  			}
					  			printf("$zero,$t%d\n",temp_index%10);
							}
							else { // non-special cases mult
								int binary[20]; // Array that store binary number inversly
								int neg = 0; // 1 if is negative
								int num;
								if (var[0]=='-'){neg=1;} // see if neg number
								num = atoi(var);
								if (num<0){
									num = num*(-1); // change to positive
								}
								// Loop to get binary number
								int k = 0;
								while (num!=0){
									binary[k] = num % 2;
									num = num / 2;
									k++;
								}
								k--;
								// Find which power is 1
								// Print out the SLL and add parts
								int first_mult = 0; // flag to see use move or add
								int j;
								for (j=0;k>0;j++,k--){
									if (binary[j]==1){
										printf("sll $t%d,",temp_index%10);
										t_used = 1;
										for (i=0;i<array_index;i++){ // find the right s
							  				if (all_var_c[i]==line[line_index][var_index1]){
							  					printf("$s%d,",i);
							  					break;
							  				}
							  			}
							  			printf("%d\n",k); // Number of moved position
							  			if (first_mult==0){ // use move
							  				printf("move $t%d,$t%d\n",(temp_index+1)%10,temp_index%10);
							  			}
							  			else{ // use add
							  				printf("add $t%d,$t%d,$t%d\n",(temp_index+1)%10,(temp_index+1)%10,temp_index%10);
							  			}
							  			first_mult = 1;// change flag
									}
								}
								// Add
								printf("add $t%d,$t%d,$s%d\n",(temp_index+1)%10,(temp_index+1)%10,i);
								// Last Move if not neg
								if (neg==0){
									printf("move ");
									for (i=0;i<array_index;i++){ // find the right s
						  				if (all_var_c[i]==line[line_index][0]){
						  					printf("$s%d,",i);
						  					break;
						  				}
						  			}
						  			printf("$t%d\n",(temp_index+1)%10);
						  		}
					  			else if (neg==1){ // if mult a neg number
					  				for (i=0;i<array_index;i++){ // find the right s
						  				if (all_var_c[i]==line[line_index][0]){
						  					printf("sub $s%d,",i);
						  					break;
						  				}
						  			}
					  				printf("$zero,$t%d\n",(temp_index+1)%10);
					  			}
					  		}
						}
						// Constant Division
						else if (line[line_index][temp_sign_index]=='/'){
							int i;
							flag_for_first_line = 1;
							// Corner cases
							// if divide 1
							if (var[0]=='1'){
								printf("move ");
								if (line[line_index][var_index2+1]==';'){
									for (i=0;i<array_index;i++){ // find the right s
						  				if (all_var_c[i]==line[line_index][0]){
						  					printf("$s%d,",i);
						  					break;
						  				}
						  			}
						  		}
						  		else{
						  			printf("$t%d,",temp_index%10);
						  		}
					  			for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$s%d\n",i);
					  					break;
					  				}
					  			}
							}
							// if divide -1
							else if (var[0]=='-' && var[1]=='1' && !isdigit(var[2])){
								printf("sub ");
								if (line[line_index][var_index2+1]==';'){
									for (i=0;i<array_index;i++){ // find the right s
						  				if (all_var_c[i]==line[line_index][0]){
						  					printf("$s%d,",i);
						  					break;
						  				}
						  			}
						  		}
								else{
									printf("$t%d,",temp_index%10);
								}
					  			for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$zero,$s%d\n",i);
					  					break;
					  				}
					  			}
							}
							// Normal division
							else{
								int neg = 0; // flag 1 if is neg
								t_used = 1;
								int j = 0;
								int temp_var = atoi(var);
								if (temp_var<0){
									neg = 1;
									temp_var = temp_var*(-1);
								}
								// If is power of 2
								if (temp_var%2==0){
									while (temp_var!=1){
										temp_var /= 2;
										j++;
									}
									printf("bltz ");
									if (line[line_index][var_index2+1]==';'){
										for (i=0;i<array_index;i++){ // find the right s
							  				if (all_var_c[i]==line[line_index][var_index1]){
							  					printf("$s%d,",i);
							  					break;
							  				}
						  				}
						  			}
						  			else{
						  				printf("bltz $t%d\n",temp_index%10);
						  			}
					  				printf("L%d\n",L_index);
					  				L_index++;
					  				printf("srl $s%d,$s%d,%d\n",i+1,i,j);
					  				if (neg==1){
					  					printf("sub $s%d,$zero,$s%d\n",i+1,i+1);
					  				}
									printf("j L%d\n",L_index);
									printf("L%d:\n",L_index-1);
																	}
								// Every division prints
								printf("li $t%d,%s\n",temp_index%10,var);
								printf("div ");
								for (i=0;i<array_index;i++){ // find the right s
					  				if (all_var_c[i]==line[line_index][var_index1]){
					  					printf("$s%d,$t%d\n",i,temp_index%10);
					  					break;
					  				}
				  				}
				  				if (line[line_index][var_index2+1]==';'){
					  				for (i=0;i<array_index;i++){ // find the right s
						  				if (all_var_c[i]==line[line_index][0]){
						  					printf("mflo $s%d\n",i);
						  					break;
						  				}
					  				}
					  			}

					  			else{
					  				temp_index++;
					  				printf("mflo $t%d\n",temp_index%10);
					  			}
				  				// Print the L label if divide power of 2
				  				temp_var = atoi(var);
								if (temp_var%2==0){
									printf("L%d:\n",L_index);
									L_index++;

								}
							}
						}
					}
				}




		  		// get to next sign and var
			  	var_index1 = var_index2;
			  	if (t_used==1){
			  		temp_index++;
			  	}
			  	int i;
			  	for (i=0;i<4;i++){
			  		sign_index++;
			  		var_index2++;
			  	}
		  	}
		}
		line_index++;
	}
	return EXIT_SUCCESS;
}
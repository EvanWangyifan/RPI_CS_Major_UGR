#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct mips 
{ 
	char line[100];
   	char sign[5];
   	char r1[10];
   	char r2[10];
   	char r3[10];

   	int initial_line_num; // the initial line number
   	int have_bubble; // if the nop line already has *
   	int start_print; // Count down of start printing
   	int status; // current status: 0-6 indicate IF, ID, ., *,...
   	int* waiting_for;
};

// Function that compares two char array
// return 1 if the same, 0 if not
int same_array(char a[10], char b[10]){
	int i = 0;
	while (a[i]!='\0' || b[i]!='\0'){
		// Continue if the same
		if (a[i]==b[i]){
			i++;
		}
		else{ // return 0 if not the same
			return 0;
		}
	}
	return 1;
}


// Function that take in array of mips object
// Produce an array of 2 digit line numbers (indicating 2 lines) that has data hazard
// Link the waiting for hazard to the target status of waiting
void find_hazards(struct mips all[5], int hazards[5][2]){
	int i,j,k;
	// Compare all lines that could have hazard
	k = 0; // index of array hazard
	for (i=0;i<3;i++){
		for (j=i+1;j<i+3;j++){
			// if the later object's r2 or r3 uses the previous's r1
			if (same_array(all[i].r1, all[j].r2) || same_array(all[i].r1, all[j].r3)){
				hazards[k][0] = i;
				hazards[k][1] = j;
				all[j].waiting_for = &(all[i].status); // Link the waiting to the line it waits for
				k++;
			}
		}
	}
}

// Function that add nop to the array of struct and move all later object one later
void add_nop(struct mips all[5], int nop_spot){
	int i,j;
	for (i=4; i>nop_spot; i--){
		for (j=0;j<100;j++){
			all[i].line[j] = all[i-1].line[j];
		}
		for (j=0;j<5;j++){
			all[i].sign[j] = all[i-1].sign[j];
		}
		for (j=0;j<10;j++){
			all[i].r1[j] = all[i-1].r1[j];
			all[i].r2[j] = all[i-1].r2[j];
			all[i].r3[j] = all[i-1].r3[j];
		}
		all[i].start_print = all[i-1].start_print;
		all[i].status = all[i-1].status;
		all[i].waiting_for = all[i-1].waiting_for;
		all[i].initial_line_num = all[i-1].initial_line_num;
	}
	// add nop
	all[nop_spot].line[0] = 'n';
	all[nop_spot].sign[0] = 'n';
	all[nop_spot].line[1] = 'o';
	all[nop_spot].sign[0] = 'o';
	all[nop_spot].line[2] = 'p';
	all[nop_spot].sign[0] = 'p';
	all[nop_spot].line[3] = '\0';
	all[nop_spot].sign[3] = '\0';
	all[nop_spot].have_bubble = 0;
}


// update a cycle
void update(struct mips all[5], int* add){
	int i;
	for (i=0; i<5; i++){
		if (all[i].line[0] != ' ' && all[i].line[0] != 'n'){
			if (all[i].start_print!=0){ 
				all[i].start_print--; // near to start
			}
			else{// the line has started
				if (all[i].status==2 && all[i].waiting_for!=NULL && *(all[i].waiting_for)<6){ // Need to add nop
					add_nop(all,i);
					if (all[i-1].line[0]=='n'){
						all[i].initial_line_num++;
					}
					(*add)++;
					break; // Don't add status
				}
				if (all[i].status==5){
					all[i].status++;
					all[i].status++;
				}
				else{
					all[i].status++;
				}
			}
		}	
	}
}

// Print the grid depend on the number of cycle
void print_grid(int grid[5][9], struct mips all[5], int cycle){
	int i,j,k;
	for (i=0; i<5; i++){
		if (all[i].line[0] != ' '){
			if (all[i].line[0] != 'n'){ // Normal cases
				grid[i][cycle] = all[i].status;
			}
			else{ // for nop
				if (all[i].have_bubble == 0){ // move down all grid content if adding new nop
					for (j=4;j>i;j--){
						for (k=0;k<=cycle;k++){
							grid[j][k] = grid[j-1][k];
						}
					}
				}
				grid[i][cycle] = 6;
				all[i].have_bubble = 1;
			}
		}
	}

	// change all extra * to . in the grid
	for (i=0; i<5; i++){
		if (all[i].line[0] == 'n'){
			if (cycle >= all[i].initial_line_num+5 && grid[i][cycle]==6){
				grid[i][cycle] = 0;
			}
		}
	}

	// Print the grid
	printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
	for (i=0; i<5; i++){
		if (all[i].sign[0] != ' '){
			if (all[i].line[0] == 'n'){
				printf("nop\t\t");
				for (j=0; j<9; j++){
					if (grid[i][j]==0 || grid[i][j]>6){
						printf(".");
					}
					else if (grid[i][j]==1){
						printf("IF");
					}
					else if (grid[i][j]==2){
						printf("ID");
					}
					else if (grid[i][j]==3){
						printf("EX");
					}
					else if (grid[i][j]==4){
						printf("MEM");
					}
					else if (grid[i][j]==5){
						printf("WB");
					}
					else if (grid[i][j]==6){
						printf("*");
					}
					if (j!=8){
						printf("\t");
					}
					else{
						printf("\n");
					}
				}
			}
			else{
				printf("%s\t",all[i].line);
				for (j=0; j<9; j++){
					if (grid[i][j]==0 || grid[i][j]>6){
						printf(".");
					}
					else if (grid[i][j]==1){
						printf("IF");
					}
					else if (grid[i][j]==2){
						printf("ID");
					}
					else if (grid[i][j]==3){
						printf("EX");
					}
					else if (grid[i][j]==4){
						printf("MEM");
					}
					else if (grid[i][j]==5){
						printf("WB");
					}
					else if (grid[i][j]==6){
						printf("*");
					}
					if (j!=8){
						printf("\t");
					}
					else{
						printf("\n");
					}
				}
			}
		}
	}

}

int main(int argc, char* argv[]){
	// Storing all objects
	struct mips all_commands[5];
	int i,j,k = 0;
	// Initialization
	for(i=0; i<5; i++){
		for (j=0;j<99;j++){
			all_commands[i].line[j] = ' ';
		}
		all_commands[i].line[j] = '\0';
		for (j=0;j<4;j++){
			all_commands[i].sign[j] = ' ';
		}
		all_commands[i].sign[j] = '\0';
		for (j=0;j<9;j++){
			all_commands[i].r1[j] = ' ';
			all_commands[i].r2[j] = ' ';
			all_commands[i].r3[j] = ' ';
		}
		all_commands[i].r1[j] = '\0';
		all_commands[i].r2[j] = '\0';
		all_commands[i].r3[j] = '\0';
	}

	char line[100];
	// Read the file and store it in 2D array
	FILE *file = fopen(argv[1],"r");
	int count = 0; // count how many valid lines
	while (fgets(line,100,file)){
		// All temp variables
		struct mips temp;
	   	char sign[5];
	   	char r1[10];
	   	char r2[10];
	   	char r3[10];
		int i,j;
		// Change last '\n' to '\0'
		i = 0;
		while (line[i] != '\n'){
			i++;
		}
		line[i] = '\0';

		// Read the line store into the object
		i = 0;

		// Read sign
		j = 0;
		while (line[i] != ' '){
			sign[j] = line[i];
			i++;
			j++;
		}
		sign[j] = '\0';
		i++;

		// Read r1
		j = 0;
		while (line[i] != ','){
			r1[j] = line[i];
			i++;
			j++;
		}
		r1[j] = '\0';
		i++;

		// Read r2
		while (line[i]!='$'){
			i++;
		}
		j = 0;
		while (line[i] != ',' && line[i] != '\0' && line[i] != ')'){
			r2[j] = line[i];
			i++;
			j++;
		}
		r2[j] = '\0';
		i++;

		// Read r3
		j = 0;
		while (line[i] != '\0'){
			r3[j] = line[i];
			i++;
			j++;
		}
		r3[j] = '\0';
		i++;


		// Assign the attributes
		for (i=0;i<100;i++){
			temp.line[i] = line[i];
		}
		for (i=0;i<5;i++){
			temp.sign[i] = sign[i];
		}
		for (i=0;i<10;i++){
			temp.r1[i] = r1[i];
		}
		for (i=0;i<10;i++){
			temp.r2[i] = r2[i];
		}
		for (i=0;i<10;i++){
			temp.r3[i] = r3[i];
		}
		
		temp.status = 0;
		temp.waiting_for = NULL;
		temp.start_print = count;
		temp.have_bubble = 0;
		temp.initial_line_num = k;
		// Store object into the array
		all_commands[k] = temp;
		k++;
		if (line[0]!=' '){
			count++;
		}
	}


	// The result grid containing the output information
	// Initialize it with '.' (0 in the grid)
	int grid[5][9];
	// 0 for '.'
	// 1 for 'IF'
	// 2 for 'ID'
	// 3 for 'EX'
	// 4 for 'MEM'
	// 5 for 'WB'
	// 6 for '*'
	for (i=0;i<5;i++){
		for(j=0;j<9;j++){
			grid[i][j] = 0;
		}
	}

	int all_hazards[5][2]; // storing all lines have hazard
	find_hazards(all_commands, all_hazards);

	printf("START OF SIMULATION\n\n");

	k=0;
	for (i=0;i<5;i++){
		if (all_commands[i].line[0]!=' '){
			k++;
		}
	}
	i=0;
	j=k-1;
	while (i<5+j){
		update(all_commands,&j);
		print_grid(grid,all_commands,i);
		i++;
		printf("\n");
	}
	
	printf("END OF SIMULATION\n");
	return EXIT_SUCCESS;
}
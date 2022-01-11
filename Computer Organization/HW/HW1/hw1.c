/* hw1.c */
/* NAME: <Yifan Wang> */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Function that intake the 2d array and print out the matrix in format
void print(int row, int col, int matrix[row][col]){
  // Get the max length of the int in matrix
  int length[row][col];
  int i, j;
  int max[col]; // Max length for each column
  for (i=0; i<col; i++){
    max[i] = 0; // Set all to 0
  }
  for (i=0; i<row; i++){
    for (j=0; j<col; j++){
      // Find length of individual int
      char find_len[100];
      sprintf(find_len, "%d", matrix[i][j]);
      length[i][j] = strlen(find_len);
    }
  }
  // Find max lengths for each column
  for (i=0; i<col; i++){
    for (j=0; j<row; j++){
      char find_len[100];
      sprintf(find_len, "%d", matrix[j][i]);
      if (max[i]<strlen(find_len)){
        max[i] = strlen(find_len);
      }
    }
  }

  // Print out in format
  for (i=0; i<row; i++){
    printf("[");
    for (j=0; j<col; j++){
      // Print space in front of int
      char final_str[100];
      memset(final_str, ' ', sizeof(max[j]-length[i][j]));
      final_str[max[j]-length[i][j]] = '\0';
      printf("%s",final_str);
      // Print int
      printf("%d",matrix[i][j]);
      if (j!=col-1){
        printf(" ");
      }
    }
    printf("]\n");
  }
}

int main( int argc, char * argv[] )
{
  /* Ensure we have the correct number of command-line arguments */
  if ( argc != 5 )
  {
    fprintf( stderr, "ERROR: Invalid inputs!\n" );
    return EXIT_FAILURE;
  }

  // Scan for the size of two matrices
  int row1, col1, row2, col2;
  row1 = atoi(argv[1]);
  col1 = atoi(argv[2]);
  row2 = atoi(argv[3]);
  col2 = atoi(argv[4]);

  // Determine if the input valid or not
  if (col1 != row2){
  	fprintf(stderr, "ERROR: Invalid inputs!\n");
  	return EXIT_FAILURE;
  }

  // Create 2D arrays to store matrices
  int matrix1[row1][col1];
  int matrix2[row2][col2];

  // Ask for user input and store into 2d arrays
  int i, j, k, tmp;

  printf("Please enter the values for the first matrix (%dx%d):\n",row1, col1);
  for (i=0; i<row1; i++){
  	for (j=0; j<col1; j++){
  		scanf("%d",&tmp);
  		matrix1[i][j] = tmp;
  	}
  }
  printf("Please enter the values for the second matrix (%dx%d):\n",row2, col2);
  for (i=0; i<row2; i++){
  	for (j=0; j<col2; j++){
  		scanf("%d",&tmp);
  		matrix2[i][j] = tmp;
  	}
  }
  printf("\n");

  // Print the multiplication process
  print(row1, col1, matrix1);
  printf("multiplied by\n");
  print(row2, col2, matrix2);
  printf("equals\n");

  // Calculating the result matrix
  int result[row1][col2];
  for (i=0; i<row1; i++){
    for (j=0; j<col2; j++){
      tmp = 0;
      for (k=0; k<col1; k++){
        tmp += matrix1[i][k]*matrix2[k][j];
      }
      result[i][j] = tmp;
    }
  }
  // Print out result
  print(row1, col2, result);

  return EXIT_SUCCESS;
}


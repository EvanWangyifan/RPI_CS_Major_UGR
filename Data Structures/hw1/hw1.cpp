//Program: Homework 1: Image Processing
//Author: Yifan Wang
//Date: Tue Jan 15th, 2019

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//This function read the original vector board and check if the input color can be dilated
//If so, change the board copied and return the new vector board
std::vector<std::string> dilation(std::vector<std::string> board, char color){
  //Copy the vector board a new vector
  std::vector<std::string> result = board;
  //Use double loop to get to every single element from the original board
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; j < board[i].size(); j++){
      //If the element is already in the new color, skip it
      if (board[i][j] == color)
        continue;
      //If the element is in the boundary and one of the adjacent element is in color, dilate it
      //Check for the element above
      if (i-1 >= 0 && board[i-1][j] == color){
        result[i][j] = color;
      }
      //Check for the element below
      else if (i+1 < board.size() && board[i+1][j] == color){
        result[i][j] = color;
      }
      //Check for the left element
      else if (j-1 >= 0 && board[i][j-1] == color){
        result[i][j] = color;
      }
      //Check for the right element
      else if (j+1 < board[i].size() && board[i][j+1] == color){
        result[i][j] = color;
      }
    }
  }
  //Return the modified board
  return result;
}

//This function take the vector board and check if any element in it can be erode
//If so, change the element from old color to new color and return the new vector board
std::vector<std::string> erosion(std::vector<std::string> board, char old_color, char new_color){
  //Copy the vector board a new vector
  std::vector<std::string> result = board;
  //Use double loop to get to every single element from the original board
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; j < board[i].size(); j++){ 
      //If the element is already in the new color, skip it
      if (board[i][j] != old_color)
        continue;
      //If the element is in the boundary and one of the adjacent element is not in old color, dilate it
      //Check for the element above
      if (i-1 >= 0 && board[i-1][j] != old_color){
        result[i][j] = new_color;
      }
      //Check for the element below
      else if (i+1 < board.size() && board[i+1][j] != old_color){
        result[i][j] = new_color;
      }
      //Check for the left element
      else if (j-1 >= 0 && board[i][j-1] != old_color){
        result[i][j] = new_color;
      }
      //Check for the right element
      else if (j+1 < board[i].size() && board[i][j+1] != old_color){
        result[i][j] = new_color;
      }
    }
  }
  //Return the modified board
  return result;
}

//This function take the original board and replace the old color with the new color
std::vector<std::string> replace(std::vector<std::string> board, char old_color, char new_color){
  //Use double for loop to get to every single element in the board
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; j < board[i].size(); j++){
      //If the element is in old color, replace it with new color
      if (board[i][j] == old_color){
        board[i][j] = new_color;
      }
    }
  }
  //Return the new board
  return board;
}

int main(int argc, char* argv[]){
  //Read the file
  std::ifstream in_str(argv[1]);
  //Print error if cannot open the file to read
  if (!in_str.good()) {
    std::cerr << "Can't open " << argv[1] << " to read.\n";
    exit(1);
  }
  //Read lines from the file and store the line to temp and add it in the end of the vector
  std::vector<std::string> board;
  std::string temp;
  while (in_str >> temp){
    board.push_back(temp);
  }
  //If the number of command line argument is not 5 or 6, report error
  if (argc != 5 && argc != 6){
    std::cerr << "Wrong command line argument" << std::endl;
  }
  //If the command line argument states replace, then use replace function
  if (std::string(argv[3]) == "replace"){
    board = replace(board, argv[4][0], argv[5][0]);
  }
  //If the command line argument states dilation, then use dilation function
  else if (std::string(argv[3]) == "dilation"){
    board = dilation(board, argv[4][0]);
  }
  //If the command line argument states erosion, then use erosion function
  else if (std::string(argv[3]) == "erosion"){
    board = erosion(board, argv[4][0], argv[5][0]);
  }
  //If other than replace, dilation, or erosion, report error
  else{
    std::cerr << "Unknown Command" << std::endl;
  }

  //Open the out file
  std::ofstream out_str(argv[2]);
  //Print error if connot open the file to write
  if (!out_str.good()) {
    std::cerr << "Can't open " << argv[2] << " to write.\n";
    exit(1);
  }

  //Write in the out file
  for (int i = 0; i < board.size(); i++){
    out_str << board[i] << std::endl;
  }

  return 0;//Everything OK
}

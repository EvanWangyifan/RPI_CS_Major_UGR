// Program: Crossword Blackout

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Simple class to record the grid location. 
class loc {
public:
  loc(int r=0, int c=0) : row(r), col(c) {}
  int row, col;
};

bool operator== (const loc& lhs, const loc& rhs) { 
  return lhs.row == rhs.row && lhs.col == rhs.col; 
}
/*
bool search(const std::vector<std::string>& grid, const std::vector& dictionary, 
  std::vector<std::vector<loc> >& all_path, std::vector<int> constraints_across, std::vector<int> constraints_down,
  std::vector<int> constraints_mix){

  std::vector<loc> path;

  if (constraints_across.size() > 0){
    for (int i=0; i<)
  }
  if (constraints_down.size() > 0){

  }
  else if (constraints_mix.size() > 0){
    
  }
*/

// The main recursive fuction finding if the right direction 
// and the down direction is right direction to find the word
bool search(loc position, const std::vector<std::string>& grid,
             const std::string& word, std::vector<loc>& path, char mode){

  // DOUBLE CHECKING OUR LOGIC: the letter at the current grid
  // position should equal the next letter in the word

  // Add current location to the path
  if (grid[position.row][position.col] == word[path.size()]){
    path.push_back(position);
  }
  else{
    return false;
  }

  // BASE CASE: if the path length matches the word length, we're done!
  if (path.size() == word.size()) return true;

  if (mode == 'A'){
    if (position.row+1>=grid.size()){
      path.pop_back();
      return false;
    }
    if (word[path.size()] == grid[position.row+1][position.col]){
      if (search(loc(position.row+1,position.col),grid,word,path,mode)){
        return true;
      }
    }
    path.pop_back();
    return false;
  }
  else if (mode == 'D'){
    if (position.col+1>=grid[0].size()){
      path.pop_back();
      return false;
    }
    if (word[path.size()] == grid[position.row][position.col+1]){
      if (search(loc(position.row,position.col+1),grid,word,path,mode)){
        return true;
      }
    }
    path.pop_back();
    return false;
  }
}




int main(int argc, char* argv[]){
	// Report error if the length of command line is wrong.
	if (argc != 5 && argc != 6){
		std::cerr << "Invalid Command" << std::endl;
	  	exit(1);
	}
	// Read the dictionary
  	std::ifstream dic_file(argv[1]);
  	// Print error if cannot open the file to read
  	if (!dic_file.good()){
    	std::cerr << "Can't open " << argv[1] << " to read.\n";
    	exit(1);
  	}

  	// Add all word in the dictionary to a vetor
  	std::vector<std::string> dic;
  	std::string line;
  	while (dic_file >> line){
  		dic.push_back(line);
  	}


	// Read the grid file
  	std::ifstream grid_file(argv[2]);
  	// Print error if cannot open the file to read
  	if (!grid_file.good()){
    	std::cerr << "Can't open " << argv[1] << " to read.\n";
    	exit(1);
  	}

  	// Add the grid to a vetor
  	// Ignore the line start with ! and +
  	// Add line start with + into another vector
  	std::vector<std::string> grid;
  	std::vector<int> constraints_across;
    std::vector<int> constraints_down;
    std::vector<int> constraints_mix;
    char constraints_mode = 'M';
  	while (grid_file >> line){
  		if (line=="!Across"||line=="!ACROSS "){
  			constraints_mode = 'A';
  		}
      else if (line=="!Down"||line=="!DOWN"){
        constraints_mode = 'D';
      }
  		else if (line[0]=='+' && constraints_mode == 'A'){
  			constraints_across.push_back(stoi(line.substr(1,line.size()-1)));
  		}
      else if (line[0]=='+' && constraints_mode == 'D'){
        constraints_down.push_back(stoi(line.substr(1,line.size()-1)));
      }
      else if (line[0]=='+' && constraints_mode == 'M'){
        constraints_mix.push_back(stoi(line.substr(1,line.size()-1)));
      }
      else if (line[0]=='!'){
        continue;
      }
  		else{
  			grid.push_back(line);
  		}
  	}


    std::vector<std::vector<loc> > all_path;



  	for (int i=0;i<dic.size();i++){
      bool found = false;

      for (int row=0;row<grid.size();row++){
        for (int col=0;col<grid[row].size();col++){
          std::vector<loc> path;
          if (grid[row][col]==dic[i][0]){
            if (constraints_mode == 'M'){
              found = search(loc(row,col), grid, dic[i], path, 'A');
              all_path.push_back(path);
              path.clear();
              //save
              found = search(loc(row,col), grid, dic[i], path, 'D');
              //save
            }
            else{
              found = search(loc(row,col), grid, dic[i], path, constraints_mode);
            }
          }
        }
      }
    }

  	/*
  	for (int i=0;i<grid.size();i++){
  		std::cout << grid[i] << std::endl;
  	}
  	for (int i=0;i<constraints.size();i++){
  		std::cout << constraints[i] << std::endl;
  	}
	*/

  	return 0;
}
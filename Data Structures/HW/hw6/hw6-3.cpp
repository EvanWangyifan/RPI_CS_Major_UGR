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

// Simple class stores 4 layer of grid
// One with all #, one with the original grid with word, 
// one shows position where across word can be added,
// and one show where down can be added 
// ('O' means can add, 'X' means no, 'S' means not for start, 
// 'E' means not for end)
class board{
public:
	board(std::vector<std::string> grid){
		empty = across_possible = down_possible = word = grid;
		for (int i=0; i<grid.size(); i++){
			for (int j=0; j<grid[0].size(); j++){
				empty[i][j] = '#';
				across_possible[i][j] = down_possible[i][j] = 'O';
			}
		}
	}
	std::vector<std::string> empty, word, across_possible, down_possible;
};

// This function take index of a vector and erase the item at that position
void vector_erase_index(std::vector<int>& v, int index){
	for (int i=index; i<v.size()-1; i++){
		v[i]=v[i+1];
	}
	v.pop_back();
}

// The function check the board and see if the word can be added
bool check_add(const board& b, std::vector<loc> start_end_point){
	// If across
	if (start_end_point[0].row == start_end_point[1].row){
		// If the start point or the end point on 'X'
		if (b.across_possible[start_end_point[0].row]
			[start_end_point[0].col]=='X' ||
			b.across_possible[start_end_point[1].row]
			[start_end_point[1].col]=='X')
			return false;
		// If the start point is on 'S' or end point on 'E'
		else if (b.across_possible[start_end_point[0].row]
			[start_end_point[0].col]=='S' ||
			b.across_possible[start_end_point[1].row]
			[start_end_point[1].col]=='E')
			return false;
		// else
		else
			return true;
	}
	// If down
	else{
		// If the start point or the end point on 'X'
		if (b.down_possible[start_end_point[0].row][start_end_point[0].col]=='X' 
		|| b.down_possible[start_end_point[1].row][start_end_point[1].col]=='X')
			return false;
		// If the start point is on 'S' or end point on 'E'
		else if (b.down_possible[start_end_point[0].row]
			[start_end_point[0].col]=='S' ||
			b.down_possible[start_end_point[1].row]
			[start_end_point[1].col]=='E')
			return false;
		// If the start point and the end point are not on 'X'
		else
			return true;
	}
}

// This take the grid vector and the dic vector, it find and return all the 
// start position and end position where the word in the dic can be added
std::vector<std::vector<loc> > find_possible_start_point
(const std::vector<std::string>& grid, const std::vector<std::string>& dic){
	std::vector<std::vector<loc> > result;
	for (int i=0; i<grid.size(); i++){
		for (int j=0; j<grid[0].size(); j++){
			for (int k=0; k<dic.size(); k++){
				// If the letter match with the first letter in the word in dic
				if(grid[i][j]==dic[k][0]){
					// Record the vertical word
					std::string temp_str="";
					for (int q=0; q<dic[k].size() && 
						i+dic[k].size()-1<grid.size();q++){
						temp_str+=grid[i+q][j];
					}
					// See if the horizontal word match with the word in dic
					if (dic[k]==grid[i].substr(j,dic[k].size())){
						std::vector<loc> temp;
						// Record the start loc
						temp.push_back(loc(i,j));
						// Record the end loc
						temp.push_back(loc(i,j+dic[k].size()-1));
						result.push_back(temp);
					}
					// See if the vertical word match with the word in dic
					if (temp_str==dic[k]){
						std::vector<loc> temp;
						// Record the start loc
						temp.push_back(loc(i,j));
						// Record the end loc
						temp.push_back(loc(i+dic[k].size()-1,j));
						result.push_back(temp);
					}
					
				}
			}
		}
	}
	// Return the board with all the possible location (start and end)
	return result;
}

// The recursive function 
// This function go through all possible locations and the constraints
// If the possible location and constraints match which means the word can be
// added to the board, Erase the possible location and the constraints.
// Then, fill all the 4 layer boards and recurse
void search
(board& grid, std::vector<int>& constraints, 
std::vector<std::vector<loc> >& possible_positions,
std::vector<std::vector<std::string> >& result){

	// Base case: all constraints are used
	if (constraints.size()==0){
		result.push_back(grid.empty);
		return;
	}

	// Start from the begin of the constraints and possible_positions
	std::vector<std::vector<loc> >::iterator itr_p = possible_positions.begin();
	// If the constraint and position satisfy the requirement, we will delete 
	// them from the constraints and store them here for later retrieve
	int temp_store_constraint;
	std::vector<loc> temp_store_possible_position;
	// Go through the possible_positions
	for (; itr_p!=possible_positions.end(); itr_p++){
		// Find the length and direction of the word in the possible position
		int length = 0;
		char direction;
		if ((*itr_p)[0].row==(*itr_p)[1].row){
			length = (*itr_p)[1].col-(*itr_p)[0].col+1;
			direction = 'A';
		}
		else{
			length = (*itr_p)[1].row-(*itr_p)[0].row+1;
			direction = 'D';
		}

		// Go through the constraints to find the same length 
		for (int j=0; j<constraints.size(); j++){
			if (constraints[j]==length){
				// store the constraint and possible position
				temp_store_constraint=constraints[j];
				temp_store_possible_position=*itr_p;
				// remove the selected constraint and possible position
				vector_erase_index(constraints, j);
				itr_p=possible_positions.erase(itr_p);
				// If can be added
				if (check_add(grid, temp_store_possible_position)){
					// Save the grid for restore
					board temp_grid = grid;
					// fill the empty board and change the across & down board
					for (int i=0; i<length; i++){
						// If the word is across
						if (direction=='A'){
							// Fill the empty board
							grid.empty[temp_store_possible_position[0].row]
							[temp_store_possible_position[0].col+i]=
							grid.word[temp_store_possible_position[0].row]
							[temp_store_possible_position[0].col+i];
							// change the across possible board
							// if the start is not at the boundary
							// make the previous letter 'X'
							if (i==0 && temp_store_possible_position[0].col!=0){
								grid.across_possible
								[temp_store_possible_position[0].row]
								[temp_store_possible_position[0].col-1]='X';
								grid.down_possible
								[temp_store_possible_position[0].row]
								[temp_store_possible_position[0].col-1]='X';
							}
							// if the end is not at the boundary
							// make the next letter 'X'
							if (i==length-1 && 
								temp_store_possible_position[0].col+
								i<grid.word[0].size()-1){
								grid.across_possible
								[temp_store_possible_position[0].row]
								[temp_store_possible_position[0].col+1]='X';
								grid.down_possible
								[temp_store_possible_position[0].row]
								[temp_store_possible_position[0].col+1]='X';
							}
							// mark the upper line not for down's end
							if (temp_store_possible_position[0].row>0){
								grid.down_possible
								[temp_store_possible_position[0].row-1]
								[temp_store_possible_position[0].col+i]='E';
							}
							// mark the lower line not for down's start
							if (temp_store_possible_position[0].row<
								grid.word.size()-1){
								grid.down_possible
								[temp_store_possible_position[0].row+1]
								[temp_store_possible_position[0].col+i]='S';
							}
							// Make the place fill with word word 'X'
							grid.across_possible
							[temp_store_possible_position[0].row]
							[temp_store_possible_position[0].col+i]='X';
						}
						// If down
						else{
							// Fill the empty board
							grid.empty[temp_store_possible_position[0].row+i]
							[temp_store_possible_position[0].col]=
							grid.word[temp_store_possible_position[0].row+i]
							[temp_store_possible_position[0].col];
							// change the across possible board
							// if the start is not at the boundary, 
							// make the previous letter 'X'
							if (i==0 && temp_store_possible_position[0].row!=0){
								grid.across_possible
								[temp_store_possible_position[0].row-1]
								[temp_store_possible_position[0].col]='X';
								grid.down_possible
								[temp_store_possible_position[0].row-1]
								[temp_store_possible_position[0].col]='X';
							}
							// if the end is not at the boundary
							// make the next letter 'X'
							if (i==length-1 && 
								temp_store_possible_position[0].row+
								i<grid.word.size()-1){
								grid.across_possible
								[temp_store_possible_position[0].row+1]
								[temp_store_possible_position[0].col]='X';
								grid.down_possible
								[temp_store_possible_position[0].row+1]
								[temp_store_possible_position[0].col]='X';
							}
							// mark the left line not for across's end
							if (temp_store_possible_position[0].row>0){
								grid.across_possible
								[temp_store_possible_position[0].row+i]
								[temp_store_possible_position[0].col-1]='E';
							}
							// mark the right line not for across's start
							if (temp_store_possible_position[0].row<
								grid.word.size()-1){
								grid.across_possible
								[temp_store_possible_position[0].row+i]
								[temp_store_possible_position[0].col+1]='S';
							}
							// Make the place fill with word word 'X'
							grid.down_possible
							[temp_store_possible_position[0].row+i]
							[temp_store_possible_position[0].col]='X';
						}
					}
					// Recurse the function and restore the grid, constraints 
					// and possible positions
					search(grid, constraints, possible_positions, result);
					grid = temp_grid;
					constraints.push_back(temp_store_constraint);
					possible_positions.push_back(temp_store_possible_position);
					return;
				}
				else{ // If cannot add
					// restore and return
					constraints.push_back(temp_store_constraint);
					possible_positions.push_back(temp_store_possible_position);
					return;
				}
			}
		}
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
  	// Store data
  	std::vector<std::string> grid;
  	std::vector<int> constraints;
  	while (grid_file >> line){
  		// Store constraints
  		if (line[0]=='+'){
  			constraints.push_back(stoi(line.substr(1,line.size()-1)));
  		}
  		// Ignore lines start with !
    	else if (line[0]=='!'){
        	continue;
    	}
    	// Store words
  		else{
  			grid.push_back(line);
  		}
  	}

  	// Check if the size of grid is smaller than the minimum word length
  	if (grid.size()<=3 && grid[0].size()<=3){
  		std::cout<<"Number of solution(s): 0"<<std::endl;
  	}
  	else{
	  	// Initialize the containers and run the recursion
	  	board grid_board = board(grid);
	  	std::vector<std::vector<std::string> > final_solution;
	  	std::vector<std::vector<loc> > possible_positions=
	  	find_possible_start_point(grid, dic);
	  	search(grid_board, constraints, possible_positions, final_solution);

	  	// Decide use which solution mode
	    if (std::string(argv[3]) == std::string("one_solution")){
	    	// remove all other solution except the first one
	    	if (final_solution.size()>0){
	    		std::vector<std::string> temp_solution = final_solution[0];
	    		final_solution.clear();
	    		final_solution.push_back(temp_solution);
	    	}
	    }
	    // If not one_solution or all_solutions report error
	    else if (std::string(argv[3])!=std::string("all_solutions")){
	    	std::cerr << "Wrong solution mode" << std::endl;
	    }

	    // Decide use which output mode
	    if (std::string(argv[4])==std::string("count_only")){
	    	std::cout<<"Number of solution(s): "<< 
	    	final_solution.size()<<std::endl;
	    }
	    else if (std::string(argv[4])==std::string("print_boards")){
	    	std::cout<<"Number of solution(s): "<< 
	    	final_solution.size()<<std::endl;
	    	for (int i=0; i<final_solution.size(); i++){
	    		std::cout<<"Board:"<<std::endl;
	    		for (int j=0;j<final_solution[i].size();j++){
	    			std::cout<<final_solution[i][j]<<std::endl;
	    		}
	    	}
	    }
	    // Not count_only or print_boards
	    else{
	    	std::cerr<<"Invalid output mode"<<std::endl;
    	}
    }

}
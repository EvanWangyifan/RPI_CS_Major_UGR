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

// Helper function to update the vector allocating rows or cols
// everytime change a combination (like add one to binary)
void update(std::vector<int>& v, int limit){
	
}

void search(std::vector<int> row, std::vector<int> col, 
	const std::vector<std::string>& grid, const std::vector<std::string>& dic,
	std::vector<std::vector<loc> >& all_path, std::vector<int> across_limit, 
	std::vector<int> down_limit, std::vector<int> mix_limit){

	// Base case when 
	bool finished = true;
	for (int i=0; i<row.size(); i++){
		// All across condition has been tried
		if(row[i]!=grid.size()-1){
			finished = false;
		}
		// All down condition has been tried
		if(col[i]!=grid[0].size()-1){
			finished = false;
		}
	}
	if (finished){
		return;
	}

	// If there is across constraint
	if (across_limit.size()>0){
		std::vector<std::vector<loc> > across_solutions;
		std::vector<loc> one_solution_across;
		// go through the across constraints
		for (int i=0; i<across_limit.size(); i++){
			// words in the dic with same length as constraint
			std::vector<std::string> words_limited_across;
			// go through the dic
			for (int j=0; j<dic.size(); j++){
				if (dic[j].size()==across_limit[i]){
					words_limited_across.push_back(dic[j]);
				}
			}
			// the line the constraint should be position at
			int line_number = row[i];
			// This stores the current start location for 
			// each rows (update after every search)
			std::vector<int> start_locations_for_rows(row.size(),0);
			// Go to the line trying to find the word
			while (start_locations_for_rows[i]+across_limit[i]<grid[0].size()){
				bool flag1=false; // if found the word in the dic
				// go through dic
				for (int k=0; k<words_limited_across.size(); k++){
					if (grid[line_number].substr(start_locations_for_rows[i], 
								across_limit[i])==words_limited_across[k]){
						// Add the location to the vector
						for (int q=0; q<across_limit[i]; q++){
							one_solution_across.push_back(loc(line_number,start_locations_for_rows[i]+q));
						}
						// update the start position for that line
						start_locations_for_rows[i]+=across_limit[i]+1;
						flag1 = true;
					}
				}
				if (flag1==false){// If not the word go one letter forward
					start_locations_for_rows[i]++;
				}
				else{ // already found the current constraint
					break;
				}
			}
		}
		if (one_solution_across.size()==across_limit.size()){
			across_solutions.push_back(one_solution_across);
		}
		update(row, grid.size()-1);
		search(row,col,grid,dic,all_path,across_limit,down_limit,mix_limit);
	}
	// For the down constraints
	if (down_limit.size()>0){
		std::vector<std::vector<loc> > down_solutions;
		std::vector<loc> one_solution_down;
		for (int i=0; i<down_limit.size(); i++){
			// words in the dic with same length as constraint
			std::vector<std::string> words_limited_down;
			// go through the dic
			for (int j=0; j<dic.size(); j++){
				if (dic[j].size()==down_limit[i]){
					words_limited_down.push_back(dic[j]);
				}
			}
			// the line the constraint should be position at
			int col_number = col[i];
			// This stores the current start location for 
			// each cols (update after every search)
			std::vector<int> start_locations_for_cols(col.size(),0);
			// Go to the line trying to find the word
			while (start_locations_for_cols[i]+down_limit[i]<grid[0].size()){
				bool flag2=false; // if found the word in the dic
				// go through dic
				std::string temp;
				for (int k=0; k<words_limited_down.size(); k++){
					temp="";
					for (int q=0; q<down_limit[i]; q++){
						temp+=grid[start_locations_for_cols[i]+q][col_number];
					}
					if (temp==words_limited_down[k]){
						// Add the location to the vector
						for (int q=0; q<down_limit[i]; q++){
							one_solution_down.push_back(loc(start_locations_for_cols[i]+q,col_number));
						}
						// update the start position for that line
						start_locations_for_cols[i]+=down_limit[i]+1;
						flag2 = true;
					}
				}
				if (!flag2){// If not the word go one letter forward
					start_locations_for_cols[i]++;
				}
				else{ // already found the current constraint
					break;
				}
			}
		}
	}
	if (one_solution_down.size()==across_limit.size()){
			down_solutions.push_back(one_solution_down);
	}
	update(col, grid[0].size()-1);
	search(row,col,grid,dic,all_path,across_limit,down_limit,mix_limit);
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

  	// Check if the size of grid is smaller than the minimum word length
  	if ((grid.size()<=3 && grid[0].size()<=3) || 
  		(grid.size()<=3 && constraints_down>0) ||
  		(grid[0].size()<=3 && constraints_across.size()>0)){
  		std::cerr << "Invalid board size" << std::endl;
  	}

    std::vector<std::vector<loc> > all_solution;
    std::vector<int> row, col;
    for (int i=0; i<constraints_across.size(); i++){
		row.push_back(0);
	}
	for (int i=0; i<constraints_down.size(); i++){
		col.push_back(0);
	}
    search(row,col,grid,dic,all_solution,constraints_across,constraints_down,
    		constraints_mix);
    // Change the board with #
    std::vector<std::vector<std::string> > empty_grid;
    for (int i=0; i<grid.size(); i++){
    	std::string temp_line="";
    	for (int j=0; j<grid[i].size(); j++){
    		temp_line+="#";
    	}
    	empty_grid.push_back(temp_line)
    }
    std::vector<std::vector<std::string> > final_solution;
    for (int i=0; i<all_solution.size(); i++){
    	final_solution.push_back(empty_grid);
    }
    for (int i=0; i<final_solution.size(); i++){
    	for (int j=0; j<final_solution[i].size(); j++){
    		for (int k=0; k<final_solution[i][j].size(); k++){
    			if (all_solution[j][k]==loc(j,k)){
    				final_solution[j][k]=grid[i][j];
    			}
    		}
    	}
    }
    // Decide use which solution mode
    if (argv[3]=="one_solution"){
    	std::vector<std::string> temp_solution = final_solution[0];
    	final_solution.clear();
    	final_solution.push_back(temp_solution);
    }
    else if (argv[3]!="all_solutions"){
    	std::cerr << "Wrong solution mode" << std::endl;
    }

    // Decide use which output mode
    if (argv[4]=="count_only"){
    	std::cout<<"Number of solution(s): "<< final_solution.size()<<std::endl;
    }
    else if (argv[4]=="print_boards"){
    	std::cout<<"Number of solution(s): "<< final_solution.size()<<std::endl;
    	for (int i=0; i<final_solution.size(); i++){
    		std::cout<<"Board:"<<std::endl;
    		for (int j=0;j<final_solution[i].size();j++){
    			std::cout<<final_solution[i][j]<<std::endl;
    		}
    	}
    }

    else{
    	std::cerr<<"Invalid output mode"<<std::endl;
    }

}
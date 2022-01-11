#include <map>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
	std::ifstream istr(argv[1]);
	std::string temp;
	std::map<std::string, int> count_mode;
	int max_value = 0;
	while(istr>>temp){
		++count_mode[temp];
	}
	for (std::map<std::string,int>::iterator itr=count_mode.begin(); itr!=count_mode.end(); itr++){
		if (itr->second > max_value){
			max_value=itr->second;
		}
	}
	for (std::map<std::string,int>::iterator itr=count_mode.begin(); itr!=count_mode.end(); itr++){
		if (itr->second == max_value){
			std::cout << itr->first << ' ';
		}
	}
	std::cout<<std::endl;

}
#include <ctime>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>


// defined in performance.cpp
void usage();


void hash_table_test(const std::string* input_data, int input_count, 
  const std::string &operation, std::string *output_data, int &output_count) {

  // HINT: declare your hash table like this:
  std::unordered_map<std::string,int> ht(input_count);
  for (int i=0; i<input_count; i++){
    ht[input_data[i]]++;
  }

  if (operation == "sort") {
    // CANNOT IMPLEMENT
  } 
  else if (operation == "remove_dups_same_order") {
    output_count = ht.size();
    std::unordered_set<std::string> temp;
    int j=0;
    // Check if already in the elements to output
    for (int i=0; i<input_count; i++){
      // If not insert to the output array and record it
      if (temp.find(input_data[i])==temp.end()){
        temp.insert(input_data[i]);
        output_data[j] = input_data[i];
        j++;
      }
    }
  } 
  else if (operation == "remove_dups_any_order") {
    output_count = ht.size();
    std::unordered_map<std::string,int>::iterator itr = ht.begin();
    // Go through the set and copy to array
    for (int i=0; itr!=ht.end(); i++,itr++){
      output_data[i] = itr->first;
    }
  } 
  else if (operation == "mode") {
    output_count = 1;
    int max = 0;
    std::string mode;
    std::unordered_map<std::string,int>::iterator itr = ht.begin();
    for (; itr!=ht.end(); itr++){
      // Find the one with highest map value and record it
      if (itr->second >= max){
        max = itr->second;
        mode = itr->first;
      }
    }
    output_data[0] = mode;
  } 
  else {
    std::cerr << "Error: Unknown operation: " << operation << std::endl;
    usage();
  }
}

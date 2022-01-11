#include <ctime>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>


// defined in performance.cpp
void usage();


void bst_test(const std::string* input_data, int input_count, 
  const std::string &operation, std::string *output_data, int &output_count) {

  // HINT: declare your binary search tree (BST) like this:
  std::set<std::string> st;
  std::map<std::string,int> mp;
  // Add input to both set and map
  for (int i=0; i<input_count; i++){
    st.insert(input_data[i]);
    mp[input_data[i]]++;
  }

  if (operation == "sort") {
    output_count = input_count;
    std::map<std::string,int>::iterator itr = mp.begin();
    int i=0;
    // Copy all item from map to the output array
    for (; itr!=mp.end(); itr++){
      // Repeat copying the time the map's second value shows
      for (int j=0; j<itr->second; j++){
        output_data[i] = itr->first;
        i++;
      }
    }
  } 
  else if (operation == "remove_dups_same_order") {
    output_count = st.size();
    std::set<std::string> temp;
    int j=0;
    for (int i=0; i<input_count; i++){
      // Check if already in the elements to output
      if (temp.find(input_data[i])==temp.end()){
        // If not insert to the output array and record it
        temp.insert(input_data[i]);
        output_data[j] = input_data[i];
        j++;
      }
    }
  } 
  else if (operation == "remove_dups_any_order") {
    output_count = st.size();
    std::set<std::string>::iterator itr = st.begin();
    // Go through the set and copy to array
    for (int i=0; itr!=st.end(); i++,itr++){
      output_data[i] = *itr;
    }
  } 
  else if (operation == "mode") {
    output_count = 1;
    int max = 0;
    std::string mode;
    std::map<std::string,int>::iterator itr = mp.begin();
    // Find the one with highest map value and record it
    for (; itr!=mp.end(); itr++){
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

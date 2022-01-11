#include <ctime>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <list>


// defined in performance.cpp
void usage();


void list_test(const std::string* input_data, int input_count, 
  const std::string &operation, std::string *output_data, int &output_count) {

  // HINT: declare your list like this:
  std::list<std::string> lst;
  // Add all data into the list
  for (int i = 0; i < input_count; i++)
    lst.push_back(input_data[i]);

  if (operation == "sort") {
    // list sort
    lst.sort();
    output_count = input_count;
    // Go through the list using iterator
    std::list<std::string>::iterator itr = lst.begin();
    for (int i=0; itr!=lst.end(); i++,itr++){
      output_data[i] = *itr;
    }
  } 
  else if (operation == "remove_dups_same_order") {
    output_count = 0;
    // First layer go through all elements in list
    std::list<std::string>::iterator itr = lst.begin();
    for (; itr!=lst.end(); itr++){
      bool dup = false;
      // Second layer to check duplicate
      for (int i=0; i<output_count; i++){
        // If already be in the output array mark it
        if (*itr == output_data[i]){
          dup = true;
          break;
        }
      }
      if (!dup){ // If not already in add it into output array
        output_data[output_count] = *itr;
        output_count++;
      }
    }
  } 
  else if (operation == "remove_dups_any_order") {
    output_count = 0;
    // Sort the list
    lst.sort();
    if (lst.size()==0){
      return;
    }
    std::string temp = lst.front();
    // Start from the second element
    std::list<std::string>::iterator itr = lst.begin();
    itr++;
    // Check two element beside each other if the same
    for (; itr!=lst.end(); itr++){
      // If the two are not the same add to the output array
      if (temp != *itr){
        output_data[output_count] = temp;
        output_count++;
      }
      temp = *itr;
    }
    if (temp != output_data[output_count]){
      output_data[output_count] = temp;
      output_count++;
    }
  } 
  else if (operation == "mode") {
    lst.sort();
    int current_count = 1;
    std::string mode;
    int mode_count = 0;
    // keep track of two pointers into the structure
    std::list<std::string>::iterator current = lst.begin();
    ++current;
    std::list<std::string>::iterator previous = lst.begin();
    for (; current != lst.end(); ++current, ++previous) {
      if (*current == *previous) {
      // if they are the same increment the count
        current_count++;
      } 
      else if (current_count >= mode_count) {
        // found a new mode!
        mode = *previous;
        mode_count = current_count;
        current_count = 1;
      } 
      else {
        current_count = 1;
      }
    }
    if (current_count >= mode_count) {
      // last entry is a new mode!
      mode = *previous;
      mode_count = current_count;
    }
    // save the mode to the output list
    output_count = 1;
    output_data[0] = mode;
  } 
  else {
    std::cerr << "Error: Unknown operation: " << operation << std::endl;
    usage();
  }
}

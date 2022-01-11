#include <ctime>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <iostream>


// defined in performance.cpp
void usage();


void priority_queue_test(const std::string* input_data, int input_count, 
  const std::string &operation, std::string *output_data, int &output_count) {

  // HINT: declare your priority_queue like this:
  // Use greater to ensure the string sort from small to big
  std::priority_queue<std::string,std::vector<std::string>,
                                std::greater<std::string> > pq;
  for (int i=0; i<input_count; i++){
    pq.push(input_data[i]);
  }
  

  if (operation == "sort") {
    output_count = input_count;
    // since already priority from small to big just add all to output array
    for (int i = 0;i<input_count;i++){
      // Add the top and then pop to read the next
      output_data[i] = pq.top();
      pq.pop();
    }
  } 
  else if (operation == "remove_dups_same_order") {
    // CANNOT IMPLEMENT
  } 
  else if (operation == "remove_dups_any_order") {
    output_count = 0;
    std::string temp = "";
    int i = 0;
    while (!pq.empty()){
      // Not read the element that has already appears
      if (pq.top()==temp){
        pq.pop();
      }
      else{
        // read the element
        output_count++;
        temp = pq.top();
        output_data[i] = pq.top();
        pq.pop();
        // Move to the next spot of output array for future adding
        i++;
      }
    }
  } 
  else if (operation == "mode") {
    output_count = 1;
    int max = 0;// max times of appearence
    int current = 1; // current times of appearence
    std::string mode = pq.top();
    std::string temp; // temp string storing what reading right now
    while (!pq.empty()){
      // If the same as the previous
      if (pq.top()==temp){
        // Add to current and read next
        current++;
        pq.pop();
      }
      // Not same as previous
      else{
        // Reset current appearence and read next
        temp = pq.top();
        current = 1;
        pq.pop();
      }
      // Check if the time of appearence more than max time of appearence
      if (current >= max){
        // If so, change max and mode
        max = current;
        mode = temp;
      }
    }
    output_data[0] = mode;
  } 
  else {
    std::cerr << "Error: Unknown operation: " << operation << std::endl;
    usage();
  }
}

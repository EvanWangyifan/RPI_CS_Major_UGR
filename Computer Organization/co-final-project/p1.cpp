/*
Team members:
Qizhi Lu
Haoyu He
Yueting Liao
Yifang Wang
Date：
*/
// All includes
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <stdio.h>
#include "Instruction.h"
#include "functions.h"



/*
void update_register( std::vector<std::pair<std::string, int> >& registers, const std::string& reg, int new_value){
  for (size_t i = 0; i < registers.size(); i++) {
    if(registers[i].first == reg){
      registers[i].second = new_value;
      return;
    }
  }

}*/

/*
The function creates registers and initialize it's value
pair contains string and int
return: vector of pair
*/
std::vector<std::pair<std::string, int> > init_registers(){
  std::vector<std::pair<std::string, int> > reg_pair;
  for (size_t i = 0; i < 8; i++) {
    std::string str = "$s";
      str += std::to_string(i);
      std::pair <std::string, int> tmp;
      tmp = make_pair(str,0);
      reg_pair.push_back(tmp);
  }

  for (size_t i = 0; i < 10; i++) {
    std::string str = "$t";
      str += std::to_string(i);
      std::pair <std::string, int> tmp;
      tmp = make_pair(str,0);
      reg_pair.push_back(tmp);
  }
  return reg_pair;
}




//Main starts here
int main(int argc, char** argv){
  // Error checkings for command line and file
  if(argc != 3){
      std::cerr << "Correct usage is " << argv[0] << " [F or N] [Input file]" << std::endl;
      return -1;
  }
  std::ifstream infile(argv[2]);
  if(!infile){
      std::cerr << "Could not open " << argv[2] << " for reading!" << std::endl;;
      return -1;
  }



  //variables declaration
  std::vector<std::pair<std::string, int> > registers = init_registers();     //creating registers, initializing with value of 0 pairs of registr(string) and value(int)
  std::vector<std::pair<std::string, int> > registers_table = init_registers();
  std::string line;
  std::vector<Instruction> input_ins;
  int line_index = 0;
  std::vector<std::pair<std::string, int> > loop_pair;
  std::pair <std::string, int> tmp;

    
  //print_registers(registers);
    //file parsing
  while (std::getline(infile, line)){
    //If the current line is Not a loop, create a class
    std::string str (",");
    std::size_t found = line.find(str);
    if (found != std::string::npos){
      input_ins.push_back(Instruction(line));
      input_ins[line_index].rd_val = rd_val(registers, input_ins[line_index].getOp(), input_ins[line_index].getRd(),
      input_ins[line_index].getReg1(), input_ins[line_index].getReg2());
      line_index++;
    }
    // If it is a loop
    else{
      tmp = make_pair(line, line_index);
      loop_pair.push_back(tmp);
    }

  }




  //****************************************************************************
  std::string mode = argv[1];
  if(mode == "F")
  std::cout << "START OF SIMULATION (forwarding)" << std::endl;
  else if (mode == "N")
  std::cout << "START OF SIMULATION (no forwarding)" << std::endl;


    // traversing through the vector and staring reading
    unsigned int count;   // index counter
    int value_1, value_2, value_3;     // dependenciy value holder
    int temp;
    for(count = 0; count < input_ins.size() - 2; ++count) {
        /* calculate the dependencies between two lines.
         一共读取三行， 分别比较第一与第二行(val1)，第二与第三行(val2)，第一与第三行(val3)。
         */
            value_1 = calculate_dependency(input_ins[count], input_ins[count+1]);
            value_2 = calculate_dependency(input_ins[count+1], input_ins[count+2]);
            value_3 = calculate_dependency(input_ins[count], input_ins[count+2]);
            //std::cout<<value_3<<"\n";

        if(count == 0) {
            input_ins[0].set_dependency(value_1);
             temp = value_3 - 1 - value_1;
            if(temp < 0 ) {
              temp =0;
            }
            input_ins[1].set_dependency(std::max(temp,value_2));
            temp = std::max(temp,value_2);
            if(temp < 0 ) {
              temp =0;
            }

        }
        else {
            int m = value_3 - temp-1;
            temp = std::max(m, value_2);
            input_ins[count+1].set_dependency(temp);
        }
    }

    // now do printing
     print(input_ins, registers_table, mode, loop_pair);

    std::cout<< "----------------------------------------------------------------------------------\nEND OF SIMULATION\n";
    //****************************************************************************
    // Check by printing

/*
    std::cout << "Loop pairs check: " << std::endl;
    for (size_t i = 0; i < loop_pair.size(); i++) {
      std::cout << loop_pair[i].first << " | "<< loop_pair[i].second << std::endl;
    }

    std::cout << "\nRegister initialization check: " << std::endl;
    print_registers(registers_table);

    std::cout << "\n\nParsing line to class check: " << std::endl;
    for (size_t i = 0; i < input_ins.size(); i++) {
      std::cout << "Op: " << input_ins[i].getOp() << std::endl;
      std::cout << "Rd: " << input_ins[i].getRd() << std::endl;
      std::cout << "Reg1: " << input_ins[i].getReg1() << std::endl;
      std::cout << "Reg2: " << input_ins[i].getReg2() << std::endl;
      std::cout<<  "Dependency is: " << input_ins[i].getdependency() << std::endl;
      std::cout << "rd_val: " << input_ins[i].rd_val << std::endl;
      std::cout << "----------------" << std::endl;
    }*/


    return 0;
}

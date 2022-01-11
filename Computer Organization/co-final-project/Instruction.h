#ifndef __INSTURCTION_
#define __INSTURCTION_
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sstream>

/*
The additional Instruction.cpp is not being made
All class functions are written in the header file
Access the private variables by getFunctions
*/
class Instruction{
public:
    Instruction(std::string line);
    //Instruction(const Instruction& j);
    std::string getOp() const{ return op; }
    std::string getRd() const { return rd; }
    std::string getReg1() const { return reg1; }
    std::string getReg2() const{ return reg2; }
    std::string getLoop() const{ return loop; }
    std::string getstr() const{
        std::string tmp;
        tmp = op + ' ' + rd + ',' + reg1 + ',' + reg2;
        return tmp; }
    int getdependency() const{ return dependency; }
    void set_dependency(int a) { dependency = a;}
    void set_loop(std::string a) {loop_name = a;}
    int rd_val;
private:
    std::string op;
    std::string rd;
    std::string reg1;
    std::string reg2;
    std::string loop;
    std::string loop_name;   // it means that this is the first line that the loop name appears
    std::string str;
    int dependency;    // indicate for nop needed if non-forwaring used
};



/*
1. Constructor read in a string line and parsing it
2. Assign the values into the private variable of the class
*/
Instruction::Instruction(std::string line){
  op = "";
  rd = "";
  reg1 = "";
  reg2 = "";
  loop = "";
  loop_name = "";
  dependency = 0;
    str = line;
  std::vector<std::string> words;
     rd_val = 0;

  //search if the line is loop only
  std::string str (",");
  std::size_t found = line.find(str);
  if (found == std::string::npos){
    loop = line;
    return;
  }
  //If it is a normal instuction liine
  // Split by comma first
  std::stringstream ss(line);
  while( ss.good() ){
      std::string substr;
      getline( ss, substr, ',' );
      words.push_back( substr );
    }
    reg1 = words[1];
    reg2 = words[2];

  words.clear();
  //Split the first index of the array, separate the op from register
  std::stringstream ss1(words[0]);
  while( ss1.good() ){
      std::string substr;
      getline( ss1, substr, ' ' );
      words.push_back( substr );
    }
    op = words[0];
    rd = words[1];

}

#endif

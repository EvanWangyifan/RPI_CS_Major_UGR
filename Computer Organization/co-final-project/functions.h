#ifndef __functions_
#define __functions_

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include "Instruction.h"


void update_register( std::vector<std::pair<std::string, int> >& registers, const std::string& reg, int new_value){
  for (size_t i = 0; i < registers.size(); i++) {
    if(registers[i].first == reg){
      registers[i].second = new_value;
      return;
    }
  }

}

int rd_val( std::vector<std::pair<std::string, int> >& registers, std::string op, std::string rd, std::string reg1, std::string reg2){
  int reg1_val = -1;
  int reg2_val = -1;
  int rd_value = -1;
  int updated_val = -1;
  // check if any of the two registers is $zero
  if(op == "bne" || op == "beq"){
    for (size_t i = 0; i < registers.size(); i++) {
      if(registers[i].first == rd){
        rd_value = registers[i].second;
      }
    }
  }
  if(reg1 == "$zero"){
    reg1_val = 0;
  }
  if(reg2 == "$zero"){
    reg2_val = 0;
  }
  // check if any of the two registers is taking the value from register map
  if(reg1[0] == '$' && reg1 != "$zero"){
    for (size_t i = 0; i < registers.size(); i++) {
      if(registers[i].first == reg1){
        reg1_val = registers[i].second;
      }
    }
  }
  if(reg2[0] == '$' && reg2 != "$zero"){
    for (size_t i = 0; i < registers.size(); i++) {
      if(registers[i].first == reg2){
        reg2_val = registers[i].second;
      }
    }
  }

  // check if any of the two registers is number
  if(reg1[0] != '$' && !isalpha(reg1[0]) ){
    reg1_val = stoi(reg1);
  }
  if(reg2[0] != '$' && !isalpha(reg2[0])){
    reg2_val = stoi(reg2);
  }


  if (op == "add"){
   updated_val = reg1_val + reg2_val;
  }
  else if (op == "addi"){
   updated_val = reg1_val + reg2_val;
  }
  else if (op == "and"){
   updated_val = reg1_val & reg2_val;
  }
  else if (op == "andi"){
   updated_val = reg1_val & reg2_val;
  }
  else if (op == "or"){
   updated_val = reg1_val | reg2_val;
  }
  else if (op == "ori"){
   updated_val = reg1_val | reg2_val;
  }
  else if (op == "slt"){
    if(reg1_val < reg2_val)
    updated_val = 1;
    else{
      updated_val = 0;
    }
  }

  else if (op == "slti"){
    if(reg1_val < reg2_val)
    updated_val = 1;
    else{
      updated_val = 0;
    }
  }
  else if (op == "beq"){
   if(rd_value == reg1_val)
   updated_val = 1;
   else{
     updated_val = 0;
   }
  }
  else if (op == "bne"){
    if(rd_value != reg1_val)
    updated_val = 1;
    else{
      updated_val = 0;
    }
  }
  /*
std::cout <<  op << "\t";
if(op == "beq" || op == "bne"){
  std::cout << rd_value << "|"<< reg1_val << "->\t";
}
else{
  std::cout << reg1_val << "|"<< reg2_val << "->\t";
}
std::cout << "[" <<  updated_val << "]"<< std:: endl;
*/
update_register(registers, rd, updated_val);
//print_registers(registers);
  return updated_val;
}
int compare( std::string a, std::string b) {
    for(unsigned int i = 0; i < a.size() && i < b.size(); ++i) {
        if(i <= b.size()-1) {
            if(a[i] != b[i]) {
                return 0;   // NOT SAME
            }
        }
    }
    return 1;    // SAME
}

// calculate the dependenies between two instruction lines
int calculate_dependency(Instruction ins_1, Instruction ins_2) {
    // line 1 rd == line2 rs/rt
    if(ins_1.getRd() == ins_2.getReg1() || ins_1.getRd() == ins_2.getReg2() ) {
        return 2;
    }
    return 0;
}

// print header
void header() {
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    std::cout <<  "CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16" << std::endl;
    return;
}

/*void update_register(const Instruction& ins, std::vector<std::pair<std::string, int> >& registers) {
    for (size_t i = 0; i < registers.size(); i++) {
        if(registers[i].first == ins.getRd()) {
            registers[i].second = ins.rd_val;
        }
    }
}*/


void print_registers(const std::vector<std::pair<std::string, int> >& registers){
  int count = 1;
  for (size_t i = 0; i < registers.size(); i++) {
      if(count % 4 != 0 && registers[i].first != "$t9") {
    std::cout << registers[i].first << " = " << std::setw(14)<< std::setiosflags(std::ios::left) << registers[i].second;
      }
    if((count % 4 == 0 && count != 0) || registers[i].first == "$t9"){
    std::cout<< registers[i].first << " = " << registers[i].second << std::endl;
    }
    count++;
  }
}

// print the instructions
void print(const std::vector<Instruction>& ins,  std::vector<std::pair<std::string, int> >& registers, std::string mode, std::vector<std::pair<std::string, int> >& loop_pair) {

    //variables declaration
    unsigned int time = 1;    // t1, t2, t3 etc.
    bool stop = false; // See if the last line has already WB
    unsigned int update_line = 0;
    int first_line_done = 0;

    for(; time <= 16; ++time) {
        if (stop==1){ // If last line has already WB, break
            break;
        }
        header();
        unsigned int line_count = 0;
        int ID_lag = 0, IF_lag = 0;
        unsigned int total_nop = 0;
            unsigned int previous_loop_time= 0;

        for(unsigned int i = 0; i < ins.size() && line_count < time; ++i) {
            unsigned int total_printed = 0;
            // print the instruction line
            for(unsigned int a = 0; a < 20; ++a) {
                if(a < ins[i].getstr().size()) {
                    std::cout << ins[i].getstr()[a];
                }
                else{
                    std::cout << " ";
                }
            }
//std::cout<< "previous_loop_time: "<<previous_loop_time <<"\n";
            // print the first chunk of dots
            for(unsigned int a = 0; a < (line_count - total_nop); ++a) {
                std::cout << ".   ";
                ++total_printed;
            }

            // print the instructions
            for(unsigned int a = 1; a + line_count <= time  ; ++a) {
                if(a == 1 && total_printed < time) {
                    if(total_printed == 15) {
                        std::cout << "IF";
                    }
                    else {
                    std::cout << "IF  ";
                    }
                    ++total_printed;
                    for(int b = 0; b < IF_lag && total_printed < time; ++b) {
                        std::cout << "IF  ";
                        ++total_printed;
                    }
                    IF_lag = 0;
                }
                if(a == 2 && total_printed < time) {
                    if(line_count < previous_loop_time && total_printed >= previous_loop_time) {
                        if(total_printed == 15) {
                            std::cout << "*";
                        }
                        else {
                        std::cout << "*   ";
                        }
                        ++total_printed;
                    }
                    else {
                    std::cout << "ID  ";
                    ++total_printed;
                    for( int b = 0; b < ID_lag && total_printed < time; ++b) {
                        std::cout << "ID  ";
                        ++total_printed;
                        IF_lag = ID_lag;
                    }
                    }
                    ID_lag = 0;
                }
                if(a == 3 && total_printed < time) {
                    if(line_count < previous_loop_time && total_printed >= previous_loop_time) {
                        if(total_printed == 15) {
                            std::cout << "*";
                        }
                        else {
                        std::cout << "*   ";
                        }
                        ++total_printed;
                    }
                    else {
                    std::cout << "EX  ";
                    ++total_printed;
                    }
                }
                if(a == 4 && total_printed < time) {
                    if(line_count < previous_loop_time && total_printed >= previous_loop_time) {
                        if(total_printed == 15) {
                            std::cout << "*";
                        }
                        else {
                        std::cout << "*   ";
                        }
                        ++total_printed;
                    }
                    else {
                    std::cout << "MEM ";
                    ++total_printed;
                    }
                }
                if(a == 5 && total_printed < time) {
                    if(line_count < previous_loop_time && total_printed >= previous_loop_time && line_count != previous_loop_time -4) {
                        if(total_printed == 15) {
                            std::cout << "*";
                        }
                        else {
                        std::cout << "*   ";
                        }
                        ++total_printed;
                        //previous_loop_time = 0;
                    }
                    else {
                        if(total_printed == 15) {
                            std::cout << "WB";
                        }
                        else {
                        std::cout << "WB  ";
                        }
                        ++total_printed;
                        if (i==ins.size()-1){
                            stop=true;
                        }
                   // update_register(registers, ins[i].getRd(), ins[i].rd_val);
                        if((update_line == 0 && first_line_done==0)|| line_count >= update_line+1 ) {
                            first_line_done = 1;
                            update_line = line_count;
                            rd_val(registers,ins[i].getOp(),ins[i].getRd(), ins[i].getReg1(), ins[i].getReg2());
                        }
                        
                        if((ins[i].getOp() == "bne" || ins[i].getOp() == "beq") && ins[i].rd_val == 1) {
                                       for(unsigned m = 0; m < loop_pair.size(); ++m) {
                                           if(compare(loop_pair[m].first,ins[i].getReg2()) == 1) {
                                               previous_loop_time = total_printed-1;
                                               break;
                                    }
                                }
                        }
                    }
                }
            }

            // print the second chunk of dots
            while(total_printed < 15) {
                std::cout << ".   ";
                ++total_printed;
            }
            if(total_printed < 16) {
            std::cout << ".";
            ++total_printed;
            }
            std::cout << "\n";
            ++line_count;
            //ID_lag = 0;

            // if no forwarding used, need to print nop
            if(mode == "N") {
                if(ins[i].getdependency() == 1) {
                    ID_lag = 1;
                    IF_lag = 0;
                }
                if(ins[i].getdependency() == 2) {
                    ID_lag = 2;
                    IF_lag = 0;
                }
                for(int nop_count = 0; nop_count < ins[i].getdependency() && i + nop_count + 3 < time ; ++nop_count) {
                    std::cout << "nop                 ";
                    ++total_nop;
                    unsigned int total_printed = 0;
                    // print the first chunk of dots
                    for(unsigned int a = 0; a < i+1; ++a) {
                        std::cout << ".   ";
                        ++total_printed;
                    }

                    // print the instructions
                    for(unsigned int a = 1; a + i <= time && total_printed <= i+5+nop_count; ++a) {
                        if(a == 1 && total_printed < time) {
                            std::cout << "IF  ";
                            ++total_printed;
                        }
                        else if( (a == 2 || (a == 3 && nop_count == 1))  && total_printed < time ) {
                            std::cout << "ID  ";
                            ++total_printed;
                        }
                        else {
                            if(total_printed < time) {
                                std::cout << "*   ";
                                ++total_printed;
                            }
                        }
                    }

                    // print the second chunk of dots
                    while(total_printed < 15) {
                        std::cout << ".   ";
                        ++total_printed;
                    }
                    std::cout << ".";
                    ++total_printed;
                    std::cout << "\n";
                    ++line_count;
                }
            }
            else {
                ID_lag = 0;
                IF_lag = 0;
            }
            
            if(i >= 3 && (ins[i-3].getOp() == "bne" || ins[i-3].getOp() == "beq") && ins[i-3].rd_val == 1) {

                for(unsigned m = 0; m < loop_pair.size(); ++m) {
                    if(compare(loop_pair[m].first,ins[i-3].getReg2()) == 1) {
                        i = loop_pair[m].second - 1;
                        //std::cout<<"prev is:" <<previous_loop_time << "\n";
                        stop=false;
                        break;
                    }
                }
            }

           

        }
    // print the registers
    std::cout << std::endl;
    print_registers(registers);

    }

    // now update the registers

}



#endif /* functions_h */

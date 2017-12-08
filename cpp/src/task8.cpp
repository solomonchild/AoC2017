#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <limits>

#include "common.hpp"

enum class Op {
    Null,
    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    NE,
    Inc,
    Dec,
    If,
};

Op parse_op(const std::string& str) {
    if(str == "==") return Op::EQ;
    if(str == "!=") return Op::NEQ;
    if(str == "<") return Op::LT;
    if(str == ">") return Op::GT;
    if(str == "<=") return Op::LE;
    if(str == ">=") return Op::GE;
    if(str == "inc") return Op::Inc;
    if(str == "dec") return Op::Dec;
    else return Op::Null;
}

int exec_op(Op op, int val, int val2) {
    switch(op) {
        case(Op::EQ):   return val == val2;
        case(Op::NEQ):  return val != val2;
        case(Op::LT):   return val < val2;
        case(Op::GT):   return val > val2;
        case(Op::LE):   return val <= val2;
        case(Op::GE):   return val >= val2;
        case(Op::Inc):  return val + val2;
        case(Op::Dec):  return val - val2;
        default: return val;
    }
}

int main(int, char**) {
    std::map<std::string, int> registers;

    std::string line;
    int highest = std::numeric_limits<int>::min();
    while(std::getline(std::cin, line)) {
        auto tokens = split(line, " ", {"if"});

        auto reg = tokens[0];
        auto op = parse_op(tokens[1]);
        int how_much = std::stoi(tokens[2]);
        auto reg_cond = tokens[3];
        auto op_cond = parse_op(tokens[4]);
        int num_cond = std::stoi(tokens[5]);

        if(exec_op(op_cond, registers[reg_cond], num_cond) == 1) { 
            registers[reg] = exec_op(op, registers[reg], how_much);
            highest = std::max(highest, registers[reg]);
        }
    }
    auto it_max = std::max_element(registers.begin(), registers.end(), [](const auto& p1, const auto& p2) {
            return p1.second < p2.second;
    });
    std::cout << it_max->second << std::endl;
    //8022
    std::cout << highest << std::endl;
    //9819
}

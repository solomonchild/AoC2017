#include <vector>
#include <iostream>
#include <map>
#include <cctype>
#include <cstdint>
#include <deque>
#include <functional>

#include "common.hpp"

enum class CommandType {
    Set,
    Add,
    Sub,
    Mul,
    Nop,
    JumpNZ,
};
using Command = std::pair<CommandType, std::vector<std::string>>; 
using Commands = std::vector<Command>;
using Registers = std::map<char, int64_t>;

CommandType string_to_cmd_type(const std::string& str) {
    if(str == "set") return CommandType::Set;
    if(str == "add") return CommandType::Add;
    if(str == "mul") return CommandType::Mul;
    if(str == "nop") return CommandType::Nop;
    if(str == "sub") return CommandType::Sub;
    else return CommandType::JumpNZ;
}

auto get_val (const std::string& arg, Registers& registers) {
    if(std::isalpha(arg[0])) {
        return registers[arg[0]];
    } else return static_cast<int64_t>(std::stoll(arg));
}

void p(Registers& reg) {
    for(const auto& p : reg) {
        std::cout << "[" << p.first << "]: " << p.second << ", ";
    }
    std::cout << std::endl;
}
using Handler = std::function<void(const std::string&, Registers&)>;
int handle_basic_cmd(CommandType type, const std::vector<std::string>& args, Registers& registers, Handler mul_hook = [](const std::string& , Registers&){}) {
        int off = 0;
        switch(type) {
            case CommandType::Set: {
               char reg = args[0][0];
               registers[reg] = get_val(args[1], registers);
            } break;
            case CommandType::Add: {
               char reg = args[0][0];
               registers[reg] += get_val(args[1], registers);
            } break;
            case CommandType::Sub: {
               char reg = args[0][0];
               registers[reg] -= get_val(args[1], registers);
            } break;
            case CommandType::Nop: {
            } break;
            case CommandType::Mul: {
               char reg = args[0][0];
               registers[reg] *= get_val(args[1], registers);
               mul_hook(args[0], registers);
            } break;
           case CommandType::JumpNZ: {
                auto arg1 = get_val(args[0], registers);
                auto arg2 = get_val(args[1], registers);
                if(arg1) {
                    off = arg2 - 1; 
                }
           } break;
        }
        return off;
}

void part1(const Commands& cmds) {
    Registers registers;

    int mul_c = 0;
    bool runs = true;

    auto mul = [&mul_c](const std::string&, Registers&) {
        mul_c++;
    };
    for(size_t i = 0; i < cmds.size() && runs; i++) {
            const auto& p = cmds[i];
            auto type = p.first;
            const auto& args = p.second;
            i += handle_basic_cmd(type, args, registers, mul);
        }
    std::cout << mul_c;
}

//handmade disasembly
void part2() {
    int64_t b = 109300;
    const int64_t c = 126300;
    int64_t h = 0;
    for(; b <= c; b += 17) {
        for(int64_t i = 2; i < b; i++) {
            if(b % i == 0) {
                h++;
                break;
            }
        }
    }
    std::cout << h << std::endl;
}

int main(int, char**) {
    bool part_1 = true;
    Commands cmds;
    for(std::string line; std::getline(std::cin, line);) {
        if(line[0] == '#')
            continue;
        auto vec = split(line);
        auto cmd = string_to_cmd_type(vec[0]);
        cmds.push_back(std::make_pair(cmd, std::vector<std::string>{vec.begin() + 1, vec.end()}));
    }

    if(part_1)
        part1(cmds); 
        //8281
    else
        part2(); 
        //911, see disas 23.cpp
}

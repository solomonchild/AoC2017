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
    JumpNZ,
};
using Command = std::pair<CommandType, std::vector<std::string>>; 
using Commands = std::vector<Command>;
using Registers = std::map<char, int64_t>;

CommandType string_to_cmd_type(const std::string& str) {
    if(str == "set") return CommandType::Set;
    if(str == "add") return CommandType::Add;
    if(str == "mul") return CommandType::Mul;
    if(str == "sub") return CommandType::Sub;
    else return CommandType::JumpNZ;
}

auto get_val (const std::string& arg, Registers& registers) {
    if(std::isalpha(arg[0])) {
        return registers[arg[0]];
    } else return static_cast<int64_t>(std::stoll(arg));
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
void part2(const Commands& cmds) {
    Registers registers {{'a', 1}};

    bool runs = true;
    for(size_t i = 0; i < cmds.size() && runs; i++) {
            const auto& p = cmds[i];
            auto type = p.first;
            const auto& args = p.second;
            i += handle_basic_cmd(type, args, registers);
        }
    std::cout << registers['h'];
}

int main(int, char**) {
    bool part_1 = false;
    Commands cmds;
    for(std::string line; std::getline(std::cin, line);) {
        auto vec = split(line);
        auto cmd = string_to_cmd_type(vec[0]);
        cmds.push_back(std::make_pair(cmd, std::vector<std::string>{vec.begin() + 1, vec.end()}));
    }

    if(part_1)
        part1(cmds); 
    else
        part2(cmds); 
}

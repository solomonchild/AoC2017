#include <vector>
#include <iostream>
#include <map>
#include <cctype>
#include <cstdint>
#include <deque>
#include <functional>

#include "common.hpp"

enum class CommandType {
    Send,
    Set,
    Add,
    Mul,
    Mod,
    Receive,
    JumpGZ,
};
using Command = std::pair<CommandType, std::vector<std::string>>; 
using Commands = std::vector<Command>;
using Registers = std::map<char, int64_t>;

CommandType string_to_cmd_type(const std::string& str) {
    if(str == "snd") return CommandType::Send;
    if(str == "set") return CommandType::Set;
    if(str == "add") return CommandType::Add;
    if(str == "mul") return CommandType::Mul;
    if(str == "mod") return CommandType::Mod;
    if(str == "rcv") return CommandType::Receive;
    else return CommandType::JumpGZ;
}

auto get_val (const std::string& arg, Registers& registers) {
    if(std::isalpha(arg[0])) {
        return registers[arg[0]];
    } else return static_cast<int64_t>(std::stoll(arg));
};

void p(Registers& reg) {
    for(const auto& p : reg) {
        std::cout << "[" << p.first << "]: " << p.second << ", ";
    }
    std::cout << std::endl;
}

using Handler = std::function<void(const std::string&, Registers&)>;
int handle_basic_cmd(CommandType type, const std::vector<std::string>& args, Registers& registers, Handler send_handler, Handler receive_handler) {
        int off = 0;
        switch(type) {
            case CommandType::Send: {
                send_handler(args[0], registers);
            } break;
            case CommandType::Set: {
               char reg = args[0][0];
               registers[reg] = get_val(args[1], registers);
            } break;
            case CommandType::Add: {
               char reg = args[0][0];
               registers[reg] += get_val(args[1], registers);
            } break;
            case CommandType::Mul: {
               char reg = args[0][0];
               registers[reg] *= get_val(args[1], registers);
            } break;
            case CommandType::Mod: {
               char reg = args[0][0];
               registers[reg] %= get_val(args[1], registers);
           } break;
            case CommandType::Receive: {
                receive_handler(args[0], registers);
           } break;
           case CommandType::JumpGZ: {
                auto arg1 = get_val(args[0], registers);
                auto arg2 = get_val(args[1], registers);
                if(arg1 > 0) {
                    off = arg2 - 1; 
                }
           } break;
        }
        return off;
}

void part1(const Commands& cmds) {
    Registers registers;

    int freq = 0;
    bool runs = true;
    auto snd = [&freq](const std::string& arg, Registers& regs) {
        freq = get_val(arg, regs);
    };
    auto recv = [&freq, &runs](const std::string& arg, Registers& regs) {
        if(get_val(arg, regs) != 0) {
            std::cout << freq << std::endl;
            runs = false;
        }
    };
    for(size_t i = 0; i < cmds.size() && runs; i++) {
            const auto& p = cmds[i];
            auto type = p.first;
            const auto& args = p.second;
            i += handle_basic_cmd(type, args, registers, snd, recv);
        }
}

void part2(const Commands& cmds) {
    using Vals = std::deque<int64_t>;
    size_t idx_1 = 0;
    Vals vals_1;
    Registers registers_1{{'p', 0}};
    bool waits_1 = false;
    size_t sent_1 = 0;

    size_t idx_2 = 0;
    Vals vals_2;
    Registers registers_2{{'p', 1}};
    bool waits_2 = false;
    size_t sent_2 = 0;

    bool runs = true;
    auto step = [&runs, &cmds](size_t& i, Registers& registers, Vals& vals, bool& waits, size_t& sent, bool other_waits, Vals& other_vals) {
        const auto& p = cmds[i];
        auto type = p.first;
        const auto& args = p.second;

        auto snd_h = [&sent, &other_vals](const std::string& arg, Registers& registers){
            other_vals.push_back(get_val(arg, registers));
            sent++;
        };

        auto rcv_h = [&vals, &other_waits, &runs, &waits](const std::string& arg, Registers& registers){
               if(vals.empty()) {
                   waits = true;
                   if(other_waits)  {
                       runs = false;
                   }
               } else {
                   waits = false;
                   auto reg = arg[0];
                   auto val = vals.front(); vals.pop_front();
                   registers[reg] = val;
               }
        };

        i += handle_basic_cmd(type, args, registers, snd_h, rcv_h);
    };

    for(;runs;) {
        (void) runs;
        step(idx_1, registers_1, vals_1, waits_1, sent_1, waits_2, vals_2);
        if(!waits_1) idx_1++;
        step(idx_2, registers_2, vals_2, waits_2, sent_2, waits_1, vals_1);
        if(!waits_2) idx_2++;
    }
    std::cout << sent_2 << std::endl;
}

int main(int, char**) {
    bool part_1 = true;
    Commands cmds;
    for(std::string line; std::getline(std::cin, line);) {
        auto vec = split(line);
        auto cmd = string_to_cmd_type(vec[0]);
        cmds.push_back(std::make_pair(cmd, std::vector<std::string>{vec.begin() + 1, vec.end()}));
    }


    if(part_1)
        part1(cmds); //1187
    else  
        part2(cmds); //5969
}

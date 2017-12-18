#include <vector>
#include <iostream>
#include <map>
#include <cctype>
#include <utility>
#include <cstdint>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>
#include <sstream>
#include <cassert>

#include "common.hpp"

enum class CommandType {
    Sound,
    Set,
    Add,
    Mul,
    Mod,
    Recover,
    JumpGZ,
};

CommandType string_to_cmd_type(const std::string& str) {
    if(str == "snd") return CommandType::Sound;
    if(str == "set") return CommandType::Set;
    if(str == "add") return CommandType::Add;
    if(str == "mul") return CommandType::Mul;
    if(str == "mod") return CommandType::Mod;
    if(str == "rcv") return CommandType::Recover;
    else return CommandType::JumpGZ;
}


using Command = std::pair<CommandType, std::vector<std::string>>; 
using Commands = std::vector<Command>;
void part1(const Commands& cmds) {
    std::map<char, int64_t> registers;
    auto freq = 0U;
    auto get_val = [&registers] (const std::string& arg) {
        if(std::isalpha(arg[0])) {
            return registers[arg[0]];
        } else return static_cast<int64_t>(std::stoll(arg));
    };

    for(size_t i = 0; i < cmds.size(); i++) {
        const auto& p = cmds[i];
        auto type = p.first;
        const auto& args = p.second;
        switch(type) {
            case CommandType::Sound: {
                freq = get_val(args[0]);
            } break;
            case CommandType::Set: {
               char reg = args[0][0];
               registers[reg] = get_val(args[1]);
            } break;
            case CommandType::Add: {
               char reg = args[0][0];
               registers[reg] += get_val(args[1]);
            } break;
            case CommandType::Mul: {
               char reg = args[0][0];
               registers[reg] *= get_val(args[1]);
            } break;
            case CommandType::Mod: {
               char reg = args[0][0];
               registers[reg] %= get_val(args[1]);
           } break;
            case CommandType::Recover: {
                if(get_val(args[0]) != 0) {
                    std::cout << freq << std::endl;
                }
           } break;
           case CommandType::JumpGZ: {
                auto arg1 = get_val(args[0]);
                auto arg2 = get_val(args[1]);
                if(arg1) {
                    i += (arg2 + ((arg2 < 0) ? -1 : +1));
                }
           } break;
        }
    }
}
struct Program {
    void wait() { w.join(); }
    void start() { started = true; }

    Program(const Commands& commands, int i) 
        :     other(nullptr) 
             ,cmds(commands)
             ,started(false)
             ,sent(0)
             ,rec(0)
             ,id(i)
             ,registers{{'p',id}}

        { 
            w = std::thread([this](){
                    while(!started) ;

                    auto get_val = [this] (const std::string& arg) {
                        if(std::isalpha(arg[0])) return registers[arg[0]];
                        else return static_cast<int64_t>(std::stoll(arg));
                    };

                    for(size_t i = 0; i < cmds.size() && started; i++) {
                        const auto& p = cmds[i];
                        auto type = p.first;
                        const auto& args = p.second;
                        switch(type) {
                        case CommandType::Sound: {
                            other->send(get_val(args[0])); 
                            sent++;
                        } break;
                        case CommandType::Set: {
                            char reg = args[0][0];
                            registers[reg] = get_val(args[1]);
                        } break;
                        case CommandType::Add: {
                           char reg = args[0][0];
                           registers[reg] += get_val(args[1]);
                        } break;
                        case CommandType::Mul: {
                           char reg = args[0][0];
                           registers[reg] *= get_val(args[1]);
                        } break;
                        case CommandType::Mod: {
                           char reg = args[0][0];
                           registers[reg] %= get_val(args[1]);
                        } break;
                        case CommandType::Recover: {
                           auto r = recv();
                           auto reg = args[0][0];
                           if(r != std::numeric_limits<int64_t>::min())  {
                               registers[reg] = r;
                               rec++;
                           }
                        } break;
                        case CommandType::JumpGZ: {
                          auto arg1 = get_val(args[0]);
                          auto arg2 = get_val(args[1]);
                          if(arg1 > 0) {
                              i += arg2;
                              i -= 1;
                          }
                        } break;
                        }
                    }
            });

        }

    void send(int64_t val) {
        std::lock_guard<std::mutex> guard{q_lock};
        values.push_back(val);
        cv.notify_one();
    }

    void print_reg() {
        std::ostringstream ss;
        ss << "[" << id << "]. ";
        for(auto p : registers) {
            ss << p.first << ": " << p.second << ", ";
        }
        std::cout << ss.str() << std::endl;
    }

    int64_t recv() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait_for(lk, std::chrono::milliseconds(100), [this]{ return !values.empty();});
        if(!values.empty()) {
            std::lock_guard<std::mutex> guard{q_lock};
            auto b = values.front();
            values.pop_front();
            return b;
        }
        else {
            started = false;
            return std::numeric_limits<int64_t>::min();
        }
    }

    std::condition_variable cv;
    std::mutex m;
    std::mutex q_lock;
    std::thread w;
    std::deque<int64_t> values;
    Program* other;
    Commands cmds;
    bool started;
    int64_t sent;
    int64_t rec;
    int64_t id;
    std::map<char, int64_t> registers;
};

void part2(const Commands& cmds) {

    Program p1(cmds, 0);
    Program p2(cmds, 1);
    p1.other = &p2;
    p2.other = &p1;
    p1.start();
    p2.start();
    p1.wait();
    p2.wait();
    p1.print_reg();
    p2.print_reg();
    std::cout << "Id: " << p1.id << " s:" << p1.sent <<  " r:" << p1.rec << std::endl;
    std::cout << "Id: " << p2.id << " s:" << p2.sent << " r:" << p2.rec << std::endl;
}
void part2_sync(const Commands& cmds) {
    using Regs = std::map<char, int64_t>;
    using Vals = std::deque<int64_t>;
    size_t idx_1 = 0;
    Vals vals_1;
    Regs registers_1{{'p', 0}};
    bool waits_1 = false;
    size_t sent_1 = 0;

    size_t idx_2 = 0;
    Vals vals_2;
    Regs registers_2{{'p', 1}};
    bool waits_2 = false;
    size_t sent_2 = 0;

    bool runs = true;
    auto step = [&runs, &cmds](size_t& i, Regs& registers, Vals& vals, bool& waits, size_t& sent, bool other_waits, Vals& other_vals) {
        auto get_val = [&registers] (const std::string& arg) {
            if(std::isalpha(arg[0])) return registers[arg[0]];
            else return static_cast<int64_t>(std::stoll(arg));
        };
        const auto& p = cmds[i];
        auto type = p.first;
        const auto& args = p.second;
        switch(type) {
            case CommandType::Sound: {
                other_vals.push_back(get_val(args[0]));
                sent++;
            } break;
            case CommandType::Set: {
                char reg = args[0][0];
                registers[reg] = get_val(args[1]);
            } break;
            case CommandType::Add: {
               char reg = args[0][0];
               registers[reg] += get_val(args[1]);
            } break;
            case CommandType::Mul: {
               char reg = args[0][0];
               registers[reg] *= get_val(args[1]);
            } break;
            case CommandType::Mod: {
               char reg = args[0][0];
               registers[reg] %= get_val(args[1]);
            } break;
            case CommandType::Recover: {
               if(vals.empty()) {
                   waits = true;
                   if(other_waits)  {
                       runs = false;
                   }
               } else {
                   waits = false;
                   auto reg = args[0][0];
                   auto val = vals.front();
                   vals.pop_front();
                   registers[reg] = val;
               }
            } break;
            case CommandType::JumpGZ: {
              auto arg1 = get_val(args[0]);
              auto arg2 = get_val(args[1]);
              if(arg1 > 0) {
                  i += arg2;
                  i -= 1;
              }
            } break;
        }
    };
    for(;runs;) {
        (void) runs;
        step(idx_1, registers_1, vals_1, waits_1, sent_1, waits_2, vals_2);
        if(!waits_1)
            idx_1++;
        step(idx_2, registers_2, vals_2, waits_2, sent_2, waits_1, vals_1);
        if(!waits_2)
            idx_2++;
    }
    std::cout << sent_2 << std::endl;
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
    else { 
        part2(cmds);
        part2_sync(cmds);
    }
    //1187
    //5969
}

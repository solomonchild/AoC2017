#include <array>
#include <numeric>
#include <iostream>
#include <string>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>

#include "common.hpp"
using Array = std::array<int, 16>;
std::string print(const Array& p) {
    std::ostringstream ss;
    for(auto i : p) {
        ss << static_cast<char>(i + 'a') << "";
    }
    return ss.str();
}
struct Action {
    virtual void operator()(Array&) {}
};
struct Spin : Action {
    virtual void operator()(Array& programs) {
        std::rotate(programs.begin(), programs.end() - how_much, programs.end());
    }
    size_t how_much;
};

struct Transform : Action {
    virtual void operator()(Array& programs) {
            std::swap(programs[first_pos], programs[second_pos]);
    }
    size_t first_pos;
    size_t second_pos;
};

struct Transform2 : Action {
    virtual void operator()(Array& programs) {
        auto it_1 = std::find_if(programs.begin(), programs.end(), [this](int i){
                return (first_pos - 'a' == i);
        });
        auto it_2 = std::find_if(programs.begin(), programs.end(), [this](int i){
                return (second_pos - 'a' == i);
        });
        std::iter_swap(it_1, it_2);
    }

    char first_pos;
    char second_pos;
};

int main(int, char**) {
    Array programs;
    std::iota(programs.begin(), programs.end(), 0U);
    std::string line;
    std::getline(std::cin, line);
    auto tokens = split(line, ",");
    std::vector<Action*> actions;

    for(auto tok : tokens) {
        if(tok[0] == 's') {
            auto how_much = std::stoi(tok.substr(1));
            auto act = new Spin;
            act->how_much = how_much;
            actions.push_back(act);
        }
        else if(tok[0] == 'x') {
            auto first_pos = std::stoi(tok.substr(1, tok.find('/')));
            auto second_pos = std::stoi(tok.substr(tok.find('/') + 1));
            auto act = new Transform;
            act->first_pos = first_pos;
            act->second_pos = second_pos;
            actions.push_back(act);
        }
        else if(tok[0] == 'p') {
            auto first_pos = (tok.substr(1, tok.find('/')));
            auto second_pos = (tok.substr(tok.find('/') + 1));
            auto act = new Transform2;
            act->first_pos = first_pos[0];
            act->second_pos = second_pos[0];
            actions.push_back(act);
        }
    }
    std::vector<std::string> vec;
    uint64_t max_iter = 1'000'000'000;
    uint64_t iter = max_iter;
    while(iter--) {
        auto current = print(programs);
        auto it = std::find(vec.begin(), vec.end(), current);
        if(it == vec.end()) {
            vec.push_back(current);
        }
        else {
            std::cout << vec[max_iter % (max_iter - iter - 1)] << std::endl;
            //nlciboghmkedpfja
            break;
        }
        for(auto* act : actions) {
            (*act)(programs);
        }
    }
    print(programs);



}

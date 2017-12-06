#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

void print(const std::vector<int>& banks) {
    for(auto i : banks) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    std::istream_iterator<int> begin(std::cin);
    std::istream_iterator<int> end;
    std::vector<int> banks(begin, {});
    print(banks);
    std::map<std::string, int> known_states;
    bool iterate = true;
    auto cycles = 0U;
    while(iterate) {
        auto max_el_it = std::max_element(banks.begin(), banks.end());
        auto index = std::distance(banks.begin(), max_el_it);
        auto max_el = *max_el_it;
        *max_el_it = 0;
        while(max_el) {
            auto i = (++index)%banks.size();
            banks[i]++;
            max_el--;
        }
        std::stringstream oss;
        std::copy(banks.begin(), banks.end(), std::ostream_iterator<int>(oss));
        std::string str = oss.str();
        if(known_states.find(str) != known_states.end()) { 
            iterate = false;
            std::cout << "Diff: " << (cycles - known_states.at(str)) << std::endl;
        }
        else known_states[str] = cycles;
        cycles++;
    }
    std::cout << "Cycles total: " << cycles;
}

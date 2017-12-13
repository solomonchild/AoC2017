#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>

int main(int, char**) {
    using Ints = std::vector<int>;
    Ints layers;
    std::string line;
    for(std::string line; std::getline(std::cin, line);) {
        std::istringstream ss(line);
        std::string _;
        size_t layer;
        size_t depth;
        ss >> layer >> _ >> depth;
        if(layer >= layers.size()) {
            size_t prev_size = layers.size();
            layers.resize(layer + 1);
            std::generate(layers.begin() + prev_size, layers.end(), [](){return 0;});
        }
        layers[layer] = depth;
    }

    using Scanner = std::vector<std::pair<int, bool>>;
    auto print = [] (Scanner scanner, int pos = -1) {
        for(auto s = 0; s < static_cast<int>(scanner.size()); s++) {
            if(s == pos)
                std::cout << "(";
            std::cout << scanner[s].first;
            if(s == pos)
                std::cout << ")";
            std::cout << " ";
        }
        std::cout << std::endl;
    };
    Scanner scanner(layers.size(), std::pair<int, bool>(0, false));

    auto move = [&layers, &print] (Scanner scanner, int how_much = 1) {
        while(how_much--) {
            for(size_t i = 0; i < scanner.size(); i++) { 
                if(layers[i] > 1) {
                    if(!scanner[i].second)
                    {
                        scanner[i].first++;
                        if(scanner[i].first == layers[i] - 1)
                            scanner[i].second = true;
                    } else {
                        scanner[i].first--;
                        if(scanner[i].first == 0)
                            scanner[i].second = false;
                    }
                }
            }
        }
        return scanner;
    };

    auto severity = [&layers, &move, &print] (Scanner scanner) {
        size_t severity = 0; 
        bool caught = false;
        for(size_t pos = 0; pos < layers.size(); pos++) {
            if(scanner[pos].first == 0 && layers[pos] != 0) {
                caught = true;
                severity += pos * layers[pos];
            }
            scanner = move(scanner);
        }
        return std::make_pair(severity, caught);
    };
    auto sev = severity(move(scanner, 0));
    std::cout << "Part1: " << sev.first << std::endl;
    //748


    for(int ps = 0; ps < std::numeric_limits<int>::max(); ps++){
        if(!severity(move(scanner, ps)).second){ 
            std::cout << ps << " delay" << std::endl;
            break;
        }
    }


}

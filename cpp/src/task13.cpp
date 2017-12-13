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

    auto severity = [&layers] (int offset = 0, bool finish = true) {
        bool caught = false;
        int sev = 0;
        for(size_t i = 0u; i < layers.size(); i++) {
            if(!(i + offset) || (i + offset)%((layers[i] * 2 - 2)) == 0) {
                sev += i * layers[i];
                caught = true;
                if(!finish)
                    break;
            }
        }

        if(sev == 0 && caught) {
            return -1;
        }
        else return sev;
    };
    auto sev = severity();
    std::cout << "Part1: " << sev << std::endl;
    //748

    for(auto i = 0; i < std::numeric_limits<int>::max(); i++) {
        if(severity(i, false) == 0) {
            std::cout << i << std::endl;
            //3873662
            break;
        }
    }
}

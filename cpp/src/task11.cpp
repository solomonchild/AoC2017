#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <complex>
#include <valarray>

#include "common.hpp"

int main(int, char**) {
    std::string dir;
    std::string line;
    using Coord = std::valarray<int>;
    Coord start{0,0,0};
    std::getline(std::cin, line);
    auto strs = split(line, ",");
    auto distance = [](const Coord& arr){
        return (std::abs(arr[0]) + std::abs(arr[1]) + std::abs(arr[2])) / 2;
    }; 
    auto max = std::numeric_limits<int>::min();
    for(const auto& s : strs) {
        if(s == "n") 
            start += {0, 1, -1};
        if(s == "ne") 
            start += {1, 0, -1};
        if(s == "nw") 
            start += {-1, 1, 0};

        if(s == "s") 
            start += {0, -1, 1};
        if(s == "se") 
            start += {1, -1, 0};
        if(s == "sw") 
            start += {-1, 0, 1};
        max = std::max(distance(start), max);
    }
    std::cout << distance(start) << std::endl;
    //685
    std::cout << max << std::endl;
    //1457
}

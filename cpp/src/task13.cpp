#include <iostream>
#include <vector>
#include <string>
#include <utility>

int main(int, char**) {
    using Walls = std::vector<std::pair<int, int>>;
    Walls walls;
    while(std::cin.good()) {
        std::string _;
        size_t layer;
        size_t depth;
        std::cin >> layer >> _ >> depth;
        walls.emplace_back(layer, depth);
    }

    auto severity = [&walls] (int offset = 0, bool finish = true) {
        bool caught = false;
        int sev = 0;
        for(auto w : walls) {
            if(!(w.first + offset) || (w.first + offset)%((w.second * 2 - 2)) == 0) {
                sev += w.first * w.second;
                caught = true;
                if(!finish)
                    break;
            }
        }
        if(sev == 0 && caught) return -1;
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

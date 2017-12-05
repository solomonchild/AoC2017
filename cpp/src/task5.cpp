#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <cmath>

int main(int argc, char** argv) {
    std::string line;
    std::istream_iterator<int> ints (std::cin);
    std::istream_iterator<int> end;
    std::vector<int> nums(ints, end); 
    
    int current = 0;
    int offset = nums[current];
    size_t steps = 0;
    do {
        steps++;
        offset = nums[current];
        if(offset >= 3) 
            nums[current]--;
        else
            nums[current]++;
        current += offset;
    }
    while(current < nums.size() && current >= 0);
    std::cout << steps << std::endl;
}

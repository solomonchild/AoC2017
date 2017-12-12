
#include <iostream>
#include <string>
#include <array>
#include <iomanip>
#include <algorithm> //transform
#include <numeric> //iota
#include <iterator> //distance
#include <functional> //bit_xor

#include "common.hpp"
const bool test = false;
const int Size = test ? 5: 256;
using Array = std::array<int, Size>;

void reverse(long from, long to, Array& numbers) {
    if(from > to) {
        while(from > to) {
            std::swap(numbers[from], numbers[to]);
            from++;
            from %= numbers.size();
            to--;
            to = (to + numbers.size())% numbers.size();
        }
        if(std::abs(from - to) < static_cast<long>(numbers.size() - 1))
            while(from < to) {
                std::swap(numbers[from], numbers[to]);
                from++;
                from %= numbers.size();
                to--;
                to = (to + numbers.size())% numbers.size();
            }

    }
    else {
        auto from_it = std::next(numbers.begin(), from);
        auto to_it = std::next(numbers.begin(), to + 1);
        std::reverse(from_it, to_it);
    }
}

void print(const Array& arr, size_t current, size_t from, size_t to) {

    for(size_t i = 0; i < arr.size(); i++) {

        if(i == from)
            std::cout << "(";
        if(i == current)
            std::cout << "[";
        std::cout << arr[i];
        if(i == current)
            std::cout << "]";
        if(i == to)
            std::cout << ")";
        std::cout << " ";
    }
    std::cout << std::endl;

}
std::vector<int> convert_part1(const std::string& str) {
    auto strs = split(str, ",");
    std::vector<int> lengths;
    std::transform(strs.begin(), strs.end(), std::back_inserter(lengths), [](const std::string& str){ return std::stoi(str);});
    return lengths;
}

std::vector<int> convert_part2(const std::string& str) {
    std::vector<int> lengths(str.begin(), str.end());
    const int ints[] = {17, 31, 73, 47, 23};
    lengths.insert(lengths.end(), std::begin(ints), std::end(ints));
    return lengths;
}

int main(int, char**) {
    Array numbers;
    std::iota(numbers.begin(), numbers.end(), 0);

    std::string line;
    std::getline(std::cin, line);
    std::vector<int> lengths;
    bool part2 = true;
    if(!part2)
     lengths = convert_part1(line);
    else 
     lengths = convert_part2(line);
    
    size_t current = 0;
    size_t skip = 0;
    int times = part2 ? 64 : 1;

    while(times --) {
        for(auto len : lengths) {
            size_t to = (current + (!len ? 1:len) - 1) % numbers.size();
            reverse(current, to, numbers);
            current  += (len + skip++);
            current %= numbers.size();
        }
    }
    if(!part2) {
        std::cout << numbers[0] * numbers[1] << std::endl; //52070
    } else { 
        std::array<int, 16> hash = {};
        for(auto it = std::begin(numbers); it != std::end(numbers); std::advance(it, 16)) {
            hash[std::distance(std::begin(numbers), it)/16] = std::accumulate(it, std::next(it, 16), 0, std::bit_xor<>());
        }
        
        std::cout.flags(std::ios::hex);
        for(int i : hash) {
            std::cout << std::setw(2) << std::setfill('0') << i;
        }
        std::cout << std::endl;
        //7f94112db4e32e19cf6502073c66f9bb
    }
}


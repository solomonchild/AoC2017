#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <numeric>
#include <functional>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <iterator>

using Array = std::array<int, 256>;

auto get_hash(std::string input) {
    Array array;
    std::iota(array.begin(), array.end(), 0);
    size_t times = 64;
    size_t skip = 0;
    size_t current = 0;
    std::vector<int> codes(input.begin(), input.end());
    codes.insert(codes.end(), {17, 31, 73, 47, 23});
    while(times--) {
        for(auto len : codes) { 
            std::reverse(array.begin(), array.begin() + len);
            size_t del = (len + skip++) % array.size();
            std::rotate(array.begin(), array.begin() + del, array.end());   
            current += del;
        }
    }
    std::rotate(array.begin(), array.end() - (current % array.size()), array.end());   

    std::array<int, 16> code;
    for(auto it = array.begin(); it != array.end(); std::advance(it, 16)) {
        code[std::distance(array.begin(), it)/16] = std::accumulate(it, std::next(it, 16), 0, std::bit_xor<>()); 
    }
    std::ostringstream oss;
    for(int i : code) {
        oss << std::hex << std::setw(2) << std::setfill('0') << i;
    }
    return oss.str();
}

size_t part1(const std::string& row_ascii) {
    return std::accumulate(row_ascii.begin(), row_ascii.end(), 0U, [](size_t count, std::uint8_t chr){
            size_t c = 0;
            std::string str(1, chr);
            chr = std::stoi(str, 0, 16);
            for(size_t i = 0; i < 4; i++)  if((chr >> i) & 0x1) c++; 
            return count + c;
    });
}
using Rows = std::array<std::array<int, 128>, 128>;
auto part2(const std::string& hash, Rows& rows, size_t rownum) {

    std::array<int, 128> row{};
    for(size_t i = 0; i < hash.size(); i++) {
        std::string tmp(1, hash[i]);
        auto chr = std::stoi(tmp, 0, 16);

        for(size_t idx = 0; idx < 4; idx++) {
            bool set = ((chr >> (3-idx)) & 0x1);
            if(set) {
                row[i*4 + idx] = -1;
            }
        }
    }
    rows[rownum] = row;
    auto max_group = 0;
    if(rownum == 127) {
        std::cout << max_group << std::endl;
    }
}

int main(int, char**){
    std::string hash; 
    std::getline(std::cin, hash);
    size_t count = 0;
    Rows rows;
    for(size_t rownum = 0; rownum < 128; rownum++) {
        auto row_ascii = get_hash(hash + "-" + std::to_string(rownum));
        count += part1(row_ascii);
        part2(row_ascii, rows, rownum);
    }
    std::cout << count << std::endl;
}

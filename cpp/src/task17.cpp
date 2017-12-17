#include <vector>
#include <iostream>
#include <limits>

void print(const std::vector<int>& v, size_t pos, size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) {
    if(to == std::numeric_limits<size_t>::max()) {
        to = v.size();
    }
    for(size_t i = from; i < to; i++) {
        if(i == pos) 
            std::cout << "(";
        std::cout << v[i];
        if(i == pos) 
            std::cout << ")";
        std::cout << " ";
    }
    std::cout << std::endl;
}
int main(int, char**) {
    bool part1 = true;
    int stride;
    std::cin >> stride;
    size_t pos = 0;
    size_t inserted_at_one = 0;
    std::vector<int> buf = {0};
    size_t max_num;
    if(part1)
        max_num = 2018;
    else 
        max_num = 50'000'000;
    for(size_t num = 1; num < max_num; num++) { 
        if(part1) {
            pos = (pos + stride) % buf.size();
            buf.insert(buf.begin() + pos + 1, num);
        }
        else {
            pos = (pos + stride) % num;
            if(pos == 0)
                inserted_at_one = num;
        }
        pos++;
    }
    if(part1) {
        print(buf, pos, pos-10, pos+10);
        std::cout << buf[(pos + 1) % buf.size()];
        //926
    }
    else
        std::cout << inserted_at_one << std::endl;
        //10150888
}

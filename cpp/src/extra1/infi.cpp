#include <iostream>
#include <vector>
#include <complex>

using Array = std::vector<std::complex<int>>;
void print(const Array& vec) {
    for(auto& v: vec) {
        std::cout << v.real() << "," << v.imag() << " ";
    }
    std::cout << std::endl;
}
int main(int, char**) {
    Array arr = {};
    size_t current_idx = 0;
    size_t intersected = 0;
    while(std::cin.ignore(1,'[')) {
        size_t x = 0;
        size_t y = 0;
        if(!(std::cin >> x))
            break;
        std::cin.ignore(1,',');
        std::cin >> y;
        if(std::cin.peek() == ']') {
            std::cin.ignore(1,']');
            arr.emplace_back(x, y);
        } else {
            std::cin.ignore(1,')');
            auto c = std::complex<int>(x, y);
            c += arr[current_idx];
            size_t count = 0;
            if(std::complex<int>{x,y} != std::complex<int>{0, 0}) {
               count = std::count(arr.begin(), arr.end(), c);
            }
            arr[current_idx] = c;
            if(count >= arr.size() - 1) {
                 print(arr);
                 intersected++;
            }
            auto p = std::make_pair(arr[0], arr[1]);
            current_idx++;
            current_idx %= arr.size();
        }
    }
    std::cout << intersected << std::endl;
}

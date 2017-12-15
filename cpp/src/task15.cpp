#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>

template<int32_t Factor, short Div>
struct Generator {
    Generator(int32_t start) :prev(start) {}
    int64_t operator()() {
        do {
            prev = (prev * Factor) % 2147483647;
        } while(prev % Div != 0);
        return prev;
    }
    int64_t prev;
};

inline auto get_16 (int64_t val) {
    return (val & 0xFFFF);
}

inline std::string take_last_word(const std::string& str) {
    auto idx = str.find_last_of(" \t");
    if(idx != std::string::npos)
        return str.substr(idx + 1);
    else 
        return str;
}

int main(int, char**) {
    static const size_t factor_A = 16807;
    static const size_t factor_B = 48271;
    std::string line;
    std::getline(std::cin, line);

    auto a_start = std::stoi(take_last_word(line));
    std::getline(std::cin, line);
    auto b_start = std::stoi(take_last_word(line));

    Generator<factor_A, 4> gen_A(a_start);
    Generator<factor_B, 8> gen_B(b_start);

    auto start = std::chrono::system_clock::now();
    size_t c = 0;
    for(int64_t i = 5'000'000; i > 0; i--) {
        auto a = gen_A();
        auto b = gen_B();
        if(get_16(a) == get_16(b)) {
            c++;
        }
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << c << std::endl;
    std::cout << duration << " ms" << std::endl;
    //320


}

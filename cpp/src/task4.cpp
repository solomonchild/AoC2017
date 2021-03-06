#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

inline bool is_there_anagram(const std::set<std::string>& set, const std::string& str) {
    std::string s = str;
    std::sort(s.begin(), s.end());
    return set.count(s) != 0;
}

int main(int, char**) {
    std::string input;
    std::size_t valid = 0;
    while(std::getline(std::cin, input)) {
        std::set<std::string> set;
        std::istringstream iss(input);
        std::istream_iterator<std::string> it(iss);
        std::istream_iterator<std::string> end;
        valid++;
        while(it != end) {
            auto str = *it;
            std::sort(str.begin(), str.end());
            if(is_there_anagram(set, str)) {
                valid--;
                break;
            }
            set.insert(str);
            it++;
        }
    }
    std::cout << valid << std::endl;
    //223
}


#pragma once
#include <vector>
#include <string>

inline std::vector<std::string> split(const std::string& str, const std::string& delim = " ", const std::vector<std::string>& skip = {}) {
    size_t start {0};
    size_t end {0};
    std::vector<std::string> res;
    while(end != std::string::npos) {
        end = str.find_first_of(delim, start);
        auto sub = str.substr(start, end - start);
        if(sub != "" && std::find(skip.begin(), skip.end(), sub) == skip.end()) {
            if(*sub.begin() == ',') sub.erase(sub.begin());
            if(*sub.rbegin() == ',') sub.erase((sub.rbegin() + 1).base());
            res.push_back(sub);
        }
        start = end + 1;
    }
    return res;
}

#ifdef __MINGW32__
inline unsigned stoi(const std::string& str) {
    std::stringstream ss(str);
    unsigned i = 0;
    ss >> i;
    return i;
}
#endif

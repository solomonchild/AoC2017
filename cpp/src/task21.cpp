#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "common.hpp"

using Rules = std::map<std::string, std::string>;
using Grid = std::vector<std::string>;



struct Pattern {
    Pattern(const Grid& g) 
    : pattern(g) { }

    bool operator==(const Pattern& other) const {
        return pattern == other.pattern;
    }

    Pattern(const std::string& s) {
        if(s.find('/') == std::string::npos) {
            auto row_size = std::sqrt(s.size());
            pattern.clear();
            for(auto i = 0u; i < row_size; i++) {
                pattern.push_back(s.substr(i * row_size, row_size));
            }
        } else pattern = split(s, "/");
    }

    Grid get_square(size_t size, size_t which) {
        Grid ret;
        which--;
        size_t how_many_in_row = pattern.size() / size;
        size_t row_s = which / how_many_in_row * size;
        size_t col_s = which % how_many_in_row * size;
        std::cout << "for " << which << " " << row_s << ":" << col_s << std::endl;
        for(size_t row = 0; row < size; row++) {
            ret.push_back({pattern[row + row_s].begin() + col_s, pattern[row + row_s].begin() + col_s + size});
        }
        return ret;

    }

    std::string str(bool slashes = true) const {
        std::ostringstream oss;
        auto delim = (slashes ? "/" : "");
        std::copy(pattern.begin(), pattern.end(), std::ostream_iterator<std::string>(oss, delim));
        auto s = oss.str();
        return (slashes ? s.erase(s.size() - 1) : s);
    }

    Grid pattern;
    friend std::ostream& operator<<(std::ostream&, const Pattern&);
};

std::ostream& operator<< (std::ostream& oss, const Pattern& p) {
    for(auto r : p.pattern) {
        oss << r << std::endl;
    }
    return oss;
}

std::string find_rule(Pattern pattern, const Rules& rules) {
    auto cmpr = pattern.str(false);

    for(size_t i = 0; i < cmpr.size(); i++) {
        std::rotate(cmpr.begin(), std::next(cmpr.begin(), i), cmpr.end());
        pattern = cmpr;
        if(rules.find(pattern.str(true)) != rules.end()) 
            return rules.at(pattern.str(true));
    }
    return "";
}

Rules get_rules(std::istream& iss) {
    Rules rules;
    for(std::string line; std::getline(iss, line);) { 
        auto split_rule = split(line, " ", {"=>"});
        rules[split_rule[0]] = split_rule[1];
    }
    return rules;
}


int main(int, char**) {
    Pattern pattern = std::string(".#./..#/###");
    auto rules = get_rules(std::cin);
    auto get_rule = [&rules] (const Pattern& p) { return find_rule(p, rules); };

    [&pattern]() {
        assert(Pattern(pattern.str()) == pattern);
        assert(Pattern(pattern.str(true)) == pattern);
        assert(Pattern(pattern.str(false)) == Pattern(".#...####"));
    }();
    Pattern n (get_rule(pattern));
    std::cout << n << std::endl;
    std::cout << n.get_square(2, 1) << std::endl;
    std::cout << n.get_square(2, 2) << std::endl;
    std::cout << n.get_square(2, 3) << std::endl;
    std::cout << n.get_square(2, 4) << std::endl;
}

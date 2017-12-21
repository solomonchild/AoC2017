#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>

#include "common.hpp"

using Rules = std::map<std::string, std::string>;
using Grid = std::vector<std::string>;

struct Pattern {
    Pattern(const Grid& g) 
    : pattern(g) { }

    Pattern rotate() {
        Grid copy = pattern;
        if(copy.size() <= 3) {
            std::string l_col = std::accumulate(copy.begin(), copy.end(), std::string(), [copy](const std::string& res, const std::string& row){
                    return res + row[0];
            });
            std::string r_col = std::accumulate(copy.begin(), copy.end(), std::string(), [copy](const std::string& res, const std::string& row){
                    return res + row[row.size()-1];
            });
            std::string t_row = copy[0];
            std::string b_row = copy[copy.size()-1];
            std::for_each(t_row.rbegin(), t_row.rend(), [i=0,&copy](char chr)mutable{copy[i++][0]=chr;});
            std::for_each(b_row.rbegin(), b_row.rend(), [i=0,&copy](char chr)mutable{copy[i++][copy.size()-1]=chr;});
            copy[0] = r_col;
            copy[copy.size()-1] = l_col;
        }
        return Pattern(copy);
    }

    Pattern flip() {
        Grid g(pattern.size());
        for(size_t i = 0; i < pattern.size(); i++) {
            for(size_t j = 0; j < pattern.size(); j++) {
                g[j] += pattern[i][j];
            }
        }
        return Pattern(g);
    }

    Pattern(const std::vector<Grid>& grids) {
        if(grids.empty())
            return;
        size_t side = std::sqrt(grids.size());
        size_t height = grids[0].size();
        std::string acc;
        for(size_t i = 0; i < side*height; i++) {
            auto f = (i / height) * side;
            auto t = f+side;
            auto from = std::next(grids.begin(), f);
            auto to = std::next(grids.begin(), t);
            acc += std::accumulate(from, to, std::string(), [i,height](const std::string& res, const Grid& g){
                return res + g[i%height];
            });
        }
        init(acc);
    }

    bool operator==(const Pattern& other) const {
        return pattern == other.pattern;
    }

    void init(const std::string& s) {
        auto row_size = std::sqrt(s.size());
        pattern.clear();
        for(auto i = 0u; i < row_size; i++) {
            pattern.push_back(s.substr(i * row_size, row_size));
        }
    }

    Pattern(const std::string& s) {
        if(s.find('/') == std::string::npos) {
            init(s);
        } else {
            pattern = split(s, "/");
            init(str(false));
        }
    }

    Pattern get_square(size_t size, size_t which) {
        Grid ret;
        size_t how_many_in_row = pattern.size() / size;
        size_t row_s = which / how_many_in_row * size;
        size_t col_s = which % how_many_in_row * size;
        for(size_t row = 0; row < size; row++) {
            ret.push_back({pattern[row + row_s].begin() + col_s, pattern[row + row_s].begin() + col_s + size});
        }
        return Pattern(ret);

    }
    size_t size() const { return pattern.size(); }
    Grid grid() const { return pattern; }

    std::string str(bool slashes = true) const {
        std::ostringstream oss;
        auto delim = (slashes ? "/" : "");
        std::copy(pattern.begin(), pattern.end(), std::ostream_iterator<std::string>(oss, delim));
        auto s = oss.str();
        return (slashes && !s.empty()? s.erase(s.size() - 1) : s);
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
    if(rules.find(pattern.str(true)) != rules.end()) 
        return rules.at(pattern.str(true));
    for(size_t i = 0; i < 4; i++) {
        pattern = pattern.rotate();
        if(rules.find(pattern.str(true)) != rules.end()) 
            return rules.at(pattern.str(true));
        if(rules.find(pattern.flip().str(true)) != rules.end()) 
            return rules.at(pattern.flip().str(true));
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
        assert(Pattern(pattern) == Pattern(pattern).rotate().rotate().rotate().rotate());
        assert(Pattern("##..") == Pattern("##..").rotate().rotate().rotate().rotate());
    }();

    bool part1 = true;

    for(size_t i = 0; i < (part1 ? 5 : 18); i++) {
        int sq = 3;
        if(!(pattern.size() & 0x1)) {
            sq = 2;
        }
        auto how_many = pattern.size()*pattern.size()/(sq*sq);
        std::string acc;
        std::vector<Grid> grids;
        for(size_t i = 0; i < how_many; i++) {
            Pattern next = get_rule(pattern.get_square(sq, i));
            grids.push_back(next.grid());
        }
        Pattern new_pattern = grids;
        /*std::cout << "\n[from]\n";
        std::cout << pattern;
        std::cout << "[to]\n";
        std::cout << new_pattern;*/
        pattern = new_pattern;
    }
    auto str = pattern.str();
    auto no_of_1 = std::count(str.begin(), str.end(), '#');
    std::cout << no_of_1 << std::endl;
    //110 for part1
    //1277716 for part2
}

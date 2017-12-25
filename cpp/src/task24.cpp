#include <string>
#include <iostream>
#include <set>
#include <deque>
#include <vector>
#include <functional>
#include <algorithm>

using AdjList = std::vector<unsigned>;
using AdjLists = std::vector<AdjList>;

AdjLists read() {
    AdjLists list;
    while(std::cin.good()) {
        unsigned in;
        unsigned out;
        if(!(std::cin >> in))
                break;
        std::cin.ignore(1, '/');
        if(!(std::cin >> out))
            break;
        if(list.size() <= in) list.resize(in+1);
        if(list.size() <= out) list.resize(out+1);
        list[in].push_back(out);
        if(in != out)
            list[out].push_back(in);
    }
    return list;
}

using Path = std::deque<unsigned>;

unsigned path_calc(const Path& p) {
    unsigned ret = 0u;
    for(size_t i = 0; i < p.size(); i++) {
        ret += p[i];
    }
    return ret;
}

Path solve1(unsigned v, AdjLists lists) {
    Path max;
    for(auto u : lists[v]) {
        auto copy = lists;
        AdjList& vl = copy[v];
        AdjList& ul = copy[u];
        vl.erase(std::find(vl.begin(), vl.end(), u));
        if(v != u)
            ul.erase(std::find(ul.begin(), ul.end(), v));
        auto tmp = Path{v, u};
        auto s = solve1(u, copy);
        tmp.insert(tmp.end(), s.begin(), s.end());
        if(path_calc(tmp) > path_calc(max)) {
            max = tmp;
        }
    }
    return max;
}

Path solve2(unsigned v, AdjLists lists) {
    Path max;
    for(auto u : lists[v]) {
        auto copy = lists;
        AdjList& vl = copy[v];
        AdjList& ul = copy[u];
        vl.erase(std::find(vl.begin(), vl.end(), u));
        if(v != u)
            ul.erase(std::find(ul.begin(), ul.end(), v));
        auto tmp = Path{v, u};
        auto s = solve2(u, copy);
        tmp.insert(tmp.end(), s.begin(), s.end());
        if(tmp.size() > max.size()) {
            max = tmp;
        } else if(tmp.size() == max.size() && path_calc(tmp) > path_calc(max)) {
            max = tmp;
        }
    }
    return max;
}

int main(int, char**) {
    auto lists = read();
    for(size_t i = 0; i < lists.size(); i++) {
        std::cout << i << ": ";
        for(auto el : lists[i])
            std::cout << el << ", ";
        std::cout << std::endl;
    }
    const bool part1 = true;
    Path max;
    if(part1)
        max = solve1(0, lists);
        //1906
    else 
        max = solve2(0, lists);
        //1824

    std::cout << path_calc(max) << std::endl;
    for(size_t i = 0; i < max.size(); i++) {
        std::cout << max[i];
        if(!(i & 0x1)) {
            std::cout << "/";
        } else if(i != max.size() - 1) std::cout << "--";
    }

}

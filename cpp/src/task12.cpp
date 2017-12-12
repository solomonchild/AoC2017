#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>

#include "common.hpp"


class UnionFind {
public:
    UnionFind(size_t size = 1) { ids.resize(size); std::iota(ids.begin(), ids.end(), 0); }
    bool connected(int u, int v) {
        return parent(u) == parent(v);
    }
    void print() {
        for(auto i : ids) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;

    }
    void reg(int u) {
        if(ids.size() < static_cast<size_t>(u)) {
            auto prev_size = ids.size();
            ids.resize(u + 1);
            std::iota(ids.begin() + prev_size, ids.end(), prev_size);
        }
    }

    int parent(int u) const {
        while(u != ids[u]) {
            u = ids[u];
        }
        return u;
    }
    int count_u(int u) {
        u = parent(u);
        std::cout << u << std::endl;
        return std::count_if(ids.begin(), ids.end(), [this, u](int el){
                return parent(el) == u;
        });

    }
    int num_groups() {
        auto ids_copy = ids;
        auto num = 1;
        for(size_t i = 0; i < ids_copy.size(); i++) {
            if(parent(i) != parent(0))  {
                num++;
                connect(i, 0);
            }
        }
        std::swap(ids, ids_copy);
        return num;
    }
    void connect(int u, int v) {
        if(connected(u, v)) return;
        int pU = parent(u);
        int pV = parent(v);
        ids[pV] = pU;
    }

private:
    std::vector<int> ids;

};

int main(int, char**) {
    std::string line;
    UnionFind uf;
    while(std::getline(std::cin, line)) {
        auto strs = split(line, " ,", {"<->"});
        int connect_to = std::stoi(strs[0]);
        uf.reg(connect_to);
        std::for_each(std::next(strs.begin(), 1), strs.end(), [&uf, connect_to](std::string s) mutable {
                auto el = std::stoi(s);
                uf.reg(el);
                uf.connect(connect_to, el);
        });
    }
    std::cout << uf.count_u(0) << std::endl;
    //306
    std::cout << uf.num_groups() << std::endl;
    //200

}

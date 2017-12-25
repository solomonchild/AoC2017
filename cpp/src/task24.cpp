#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>

struct Port {
    int in;
    int out;
    Port(int in, int out)
        : in(in), out(out) {}
};
using PortMap = std::map<Port, int, std::function<bool(const Port&, const Port&)>>;
using Ports = std::vector<Port>;

std::ostream& operator<<(std::ostream& oss, const Port& p) {
    oss << p.in << " " << p.out;
    return oss;
}

PortMap read() {
    PortMap map([](const Port& p1, const Port& p2){return p1.in < p2.in || p1.out < p2.out;});
    while(std::cin.good()) {
        int in;
        int out;
        if(!(std::cin >> in))
                break;
        std::cin.ignore(1, '/');
        if(!(std::cin >> out))
            break;
        Port p(in, out);
        map[p]++;
    }
    return map;
}

int find_out (const Ports& ports) {
    if(ports.size() == 1)
        return ports[0].out;
    auto ultimate = std::prev(ports.end());
    auto pen_ultimate = std::prev(ports.end(), 2);
    if(find_out(ports) == pen_ultimate->out) return pen_ultimate->in;
    else return pen_ultimate->out;
};

void build(Ports& ports, const Port& start, PortMap& map) {
    auto out = find_out(ports);
    auto it_in = std::find_if(map.begin(), map.end(), [out](const auto& p){
            return p.second > 0 && p.first.in == start.out;
    });
    if(it_in != map.end())
        std::cout << it_in->first.in;

}

int main(int, char**) {
    auto map = read();
    std::cout << std::endl;
    for(auto& p : map) {
        std::cout << p.first << std::endl;
    }

}

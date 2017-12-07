#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <cassert>
#include "common.hpp"


struct Node{ 
    Node(const std::string& name, unsigned weight = 0) 
    : parent(nullptr),
      name(name),
      weight(weight) 
    { }
    unsigned calc_weight() const { 
        return std::accumulate(children.begin(), children.end(), weight, [](unsigned w, const Node* n2){
            return w + n2->calc_weight();
        });
    }

    Node* parent;
    std::string name;
    std::vector<Node*> children;
    unsigned weight;
};

int main(int , char** ) {
    using NamedNodes = std::map<std::string, Node*>;
    NamedNodes nodes;
    std::string line;
    while(std::getline(std::cin, line)) {
        auto strings = split(line, " ", {"->"});
        unsigned weight = stoi(strings[1].substr(1, strings[1].find(")") - 1));
        strings.erase(strings.begin() + 1);
        Node* node = nullptr;
        if(nodes.find(*strings.begin()) == nodes.end()) {
            node = nodes.insert(std::make_pair(*strings.begin(), new Node(*strings.begin(), weight))).first->second; 
        } else  {
            node = nodes[*strings.begin()];
            node->weight = weight;
        }
        for(auto it = strings.begin() + 1; it != strings.end(); it++) {
            if(nodes[*it] == nullptr) {
                nodes[*it] = new Node(*it);
            }
            node->children.push_back(nodes[*it]);
            nodes[*it]->parent = node;
        }
    }
    auto i = std::find_if(nodes.begin(), nodes.end(), [](const std::pair<std::string, Node*>& p){ return p.second->parent == nullptr;});
    std::cout << "Root: " << i->first << std::endl;
    assert(i->first == "ahnofa");

    auto root = i->second;

    //part 2
    auto find_mode = [](const std::vector<Node*>& nodes) -> std::pair<unsigned, Node*>{
        if(nodes.empty()) {
            return std::make_pair(0LL, nullptr);
        }

        auto minmax = std::minmax_element(nodes.begin(), nodes.end(), [](const Node* n1, const Node* n2){
            return n1->calc_weight() < n2->calc_weight();
        });
        auto max_el = *minmax.second;
        auto min_el = *minmax.first;
        int64_t max = (*minmax.second)->calc_weight();
        int64_t min = (*minmax.first)->calc_weight();

        auto count_weight = [] (const std::vector<Node*>& nodes, int64_t weight) { 
            return std::count_if(nodes.begin(), nodes.end(), [weight](const Node* n){
                    return n->calc_weight() == weight;
            });
       
        };
        auto max_count = count_weight(nodes, max);
        auto min_count = count_weight(nodes, min); 

        if(std::max(max_count, min_count) == max_count)  return std::make_pair(max, min_el);
        else return std::make_pair(min, max_el);
    };
    auto are_offending = [](const std::vector<Node*>& nodes) {
        auto minmax = std::minmax_element(nodes.begin(), nodes.end(), [](const Node* n1, const Node* n2){
            return n1->calc_weight() < n2->calc_weight();
        });
        int64_t max = (*minmax.second)->calc_weight();
        int64_t min = (*minmax.first)->calc_weight();
        return max != min;
    };
    auto node = root;
    auto mode = 0U;
    while(!node->children.empty() && are_offending(node->children)) {
        std::tie(mode, node) = find_mode(node->children);
    }
    auto w = (mode - node->calc_weight()) + node->weight;
    std::cout << "Offending : " << node->name << ", diff: " << w << std::endl;

}

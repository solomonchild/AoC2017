#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <sstream>
#include <cassert>
#include <algorithm>


struct Node{ 
    Node(const std::string& name, unsigned weight = 0) 
    : parent(nullptr),
      name(name),
      weight(weight) 
    { }
    unsigned calc_children_weight() const { 
        auto res = std::accumulate(children.begin(), children.end(), 0U, [](unsigned weight, const Node* n2){
            return weight + n2->weight + n2->calc_children_weight();
        });
        return res;
    }

    Node* parent;
    std::string name;
    std::vector<Node*> children;
    unsigned weight;
};

std::vector<std::string> split(const std::string& str) {
    size_t start {0};
    size_t end {0};
    std::vector<std::string> res;
    while(end != std::string::npos) {
        end = str.find(" ", start);
        auto sub = str.substr(start, end - start);
        if(sub != "->") {
            if(*sub.begin() == ',') sub.erase(sub.begin());
            if(*sub.rbegin() == ',') sub.erase((sub.rbegin() + 1).base());
            res.push_back(sub);
        }
        start = end + 1;
    }
    return res;
}


unsigned stoi(const std::string& str) {
    std::stringstream ss(str);
    unsigned i = 0;
    ss >> i;
    return i;
}

int main(int , char** ) {
    using NamedNodes = std::map<std::string, Node*>;
    NamedNodes nodes;
    std::string line;
    while(std::getline(std::cin, line)) {
        auto strings = split(line);
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

    auto root = i->second;

    //part 2
    auto find_mode = [](const std::vector<Node*>& nodes) -> std::pair<unsigned, Node*>{
        if(nodes.empty()) {
            return std::make_pair(0LL, nullptr);
        }

        auto minmax = std::minmax_element(nodes.begin(), nodes.end(), [](const Node* n1, const Node* n2){
            return (n1->weight + n1->calc_children_weight()) < (n2->weight + n2->calc_children_weight());
        });
        auto max_el = *minmax.second;
        auto min_el = *minmax.first;
        int64_t max = (*minmax.second)->weight + (*minmax.second)->calc_children_weight();
        int64_t min = (*minmax.first)->weight + (*minmax.first)->calc_children_weight();
        auto max_count = std::count_if(nodes.begin(), nodes.end(), [max](const Node* n){
                return n->weight+n->calc_children_weight() == max;
        });
        auto min_count = std::count_if(nodes.begin(), nodes.end(), [min](const Node* n){
                return n->weight+n->calc_children_weight() == min;
        });
        if(std::max(max_count, min_count) == max)  return std::make_pair(max, min_el);
        else return std::make_pair(min, max_el);
    };
    auto are_offending = [](const std::vector<Node*>& nodes) {
        auto minmax = std::minmax_element(nodes.begin(), nodes.end(), [](const Node* n1, const Node* n2){
            return (n1->weight + n1->calc_children_weight()) < (n2->weight + n2->calc_children_weight());
        });
        int64_t max = (*minmax.second)->weight + (*minmax.second)->calc_children_weight();
        int64_t min = (*minmax.first)->weight + (*minmax.first)->calc_children_weight();
        return max != min;
    };
    auto node = root;
    auto mode = 0U;
    while(!node->children.empty() && are_offending(node->children)) {
        std::tie(mode, node) = find_mode(node->children);
    }
    std::cout << "Offending : " << node->name << ", diff: " << mode - node->calc_children_weight() << std::endl;

}

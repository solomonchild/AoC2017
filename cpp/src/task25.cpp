#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cassert>

std::string get_token(const std::string& str, size_t num) {
	std::istringstream iss(str);
	std::string res;
	for(size_t _ = 0; _ < num - 1; _++)
		iss >> res;
	iss >> res;
	return res;
}


    enum class Dir {
        Right,
        Left
    };
	struct StateInfo {
        int write_val;
        int move_how_much;
        Dir move_where;
        int continue_state;
	};
using States = std::map<int, std::map<int, StateInfo>>;

std::pair<int, int> get_start_info(std::istream& iss) {
	std::string line;
	std::getline(iss, line);
	auto begin_state = get_token(line, 4)[0] - 'A';
	std::getline(iss, line);
	auto steps = std::stoi(get_token(line, 6));
    return std::make_pair(begin_state, steps);
}

States parse_input(std::istream& iss) {

    std::string line;
    auto skip_newlines = [&line, &iss] () {
        while(std::getline(iss, line) && line == "") ;
    };
	States states;
	while(iss.good()) {
		auto state = get_token(line, 3)[0] - 'A';
        skip_newlines();

		while(get_token(line, 1) == "If") {
			auto if_cur_val = std::stoi(get_token(line, 6));
			std::getline(iss, line);
			auto write = std::stoi(get_token(line, 5));

			std::getline(iss, line);
			auto move_what_tok = get_token(line, 3);
            auto move_what = 0;
            if(move_what_tok == "one") move_what = 1;
            else if(move_what_tok == "two") move_what = 2;

			auto move_where_tok = get_token(line, 7);
			move_where_tok = move_where_tok.substr(0, move_where_tok.size() - 1);
            auto move_where = (move_where_tok == "right") ? Dir::Right : Dir::Left;

			std::getline(iss, line);
			auto continue_state = get_token(line, 5)[0] - 'A';

            states[state][if_cur_val] = StateInfo{write, move_what, move_where, continue_state};
            std::getline(iss, line);
		}
	}
    return states;
}


int main(int, char**) {
    int start;
    size_t steps;
    std::tie(start, steps) = get_start_info(std::cin);
    auto states = parse_input(std::cin);
    std::vector<int> line(5, 0);
    int32_t cur_pos = 0;
    auto cur_state = start;
    auto print = [&cur_pos, &line] () {
        for(size_t i = 0; i < line.size(); i++) {
            if(i == static_cast<size_t>(cur_pos)) 
                std::cout << "[";
            std::cout << line[i];
            if(i == static_cast<size_t>(cur_pos)) 
                std::cout << "]";
            std::cout << " ";
        }
        std::cout << std::endl;
    };
    auto expand_if_needed = [&cur_pos, &line]() {
        if(cur_pos < 0) {
            line.insert(line.begin(), -cur_pos, 0);
            cur_pos = 0 ;
        } else if(static_cast<size_t>(cur_pos) > line.size() - 1) {
            line.insert(line.end(), cur_pos - line.size() + 1, 0);
        }
    };
    (void) print;
    for(size_t s = 0; s < steps; s++) {
        expand_if_needed();
        auto cur_val = line[cur_pos];
        assert(states[cur_state].find(cur_val) != states[cur_state].end());
        auto info = states[cur_state][cur_val];
        line[static_cast<size_t>(cur_pos)] = info.write_val;
        int move_where = info.move_how_much;
        if(info.move_where == Dir::Left)  {
            move_where *= -1;
        }
        cur_pos += move_where;
        cur_state = info.continue_state;
    }
    std::cout << std::count(line.begin(), line.end(), 1) << std::endl;
    //2526
}

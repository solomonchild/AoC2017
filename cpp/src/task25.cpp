#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cassert>




struct StateInfo {
    enum class Dir {
        Right,
        Left
    };
    int write_val;
    int move_how_much;
    Dir move_where;
    int continue_state;
};

using States = std::map<int, std::map<int, StateInfo>>;

template<typename T = std::string>
T get_token(std::string str, size_t num, bool trim_dot = true) {
    if(trim_dot && str[str.size() - 1] == '.') {
        str = str.substr(0, str.size() - 1);
    }

	std::istringstream iss(str);
	T res{};
    std::string temp;
	for(size_t _ = 0; _ < num - 1; _++)
		iss >> temp;
	iss >> res;
	return res;
}

template<typename T = std::string>
T get_token_after(const std::string& str, const std::string& expected) {
    auto last_token_num = std::count(expected.begin(), expected.end(), ' ') + 1;
    if(expected[0] == ' ') last_token_num--;
    if(expected[expected.size() - 1] == ' ') last_token_num--;
    return get_token<T>(str,  last_token_num + 1); 
}

std::pair<int, int> get_start_info(std::istream& iss) {
	std::string line;
	std::getline(iss, line);
	auto begin_state = get_token_after<char>(line, "Begin in state") - 'A';
	std::getline(iss, line);
	auto steps = get_token_after<int>(line, "Perform a diagnostic checksum after ");
    return std::make_pair(begin_state, steps);
}

States parse_input(std::istream& iss) {

    std::string line;
    auto skip_newlines = [&line, &iss] () {
        while(std::getline(iss, line) && line == "") ;
    };
	States states;
	while(iss.good()) {
		auto state = get_token_after<char>(line, "In state") - 'A';
        skip_newlines();

		while(get_token(line, 1) == "If") {
			auto if_cur_val = get_token_after<int>(line, "If the current value is");
			std::getline(iss, line);
			auto write = get_token_after<int>(line, "- Write the value");

			std::getline(iss, line);
			auto move_what_tok = get_token_after(line, "- Move ");
            auto move_what = 0;
            if(move_what_tok == "one") move_what = 1;
            else if(move_what_tok == "two") move_what = 2;

			auto move_where_tok = get_token_after(line, "- Move one slot to the ");
    
            auto move_where = (move_where_tok == "right") ? StateInfo::Dir::Right : StateInfo::Dir::Left;

			std::getline(iss, line);
			auto continue_state = get_token_after<char>(line, "- Continue with state") - 'A';

            states[state][if_cur_val] = StateInfo{write, move_what, move_where, continue_state};
            std::getline(iss, line);
		}
	}
    return states;
}


int main(int, char**) {
    size_t cur_state;
    size_t steps;
    std::tie(cur_state, steps) = get_start_info(std::cin);
    auto states = parse_input(std::cin);
    std::vector<int> line(5, 0);
    auto cur_pos = 0u;
#if 0
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
    (void) print;
#endif

    auto move_and_expand_if_needed = [&cur_pos, &line](int offset) {
        if(offset < 0 && cur_pos < static_cast<decltype(cur_pos)>(std::abs(offset))) {
            line.insert(line.begin(), -offset, 0);
            cur_pos = 0;
        } 
        else {
            cur_pos += offset;
            if(cur_pos > line.size() - 1) {
                line.insert(line.end(), cur_pos - line.size() + 1, 0);
            }
        }
    };

    for(size_t _ = 0; _ < steps; _++) {
        auto cur_val = line[cur_pos];
        assert(states[cur_state].find(cur_val) != states[cur_state].end());
        auto info = states[cur_state][cur_val];
        line[cur_pos] = info.write_val;
        int offset = info.move_how_much;
        if(info.move_where == StateInfo::Dir::Left)  {
            offset *= -1;
        }
        move_and_expand_if_needed(offset);
        cur_state = info.continue_state;
    }
    std::cout << std::count(line.begin(), line.end(), 1) << std::endl;
    //2526
}

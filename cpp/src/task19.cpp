#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <deque>
#include <set>
#include <cassert>


using Rows = std::vector<std::string>;
auto dfs(const Rows& rows) {

    auto valid_coord = [&rows] (int x, int y) {
        return y >= 0 && y < static_cast<int>(rows.size()) && x >=0 && x < static_cast<int>(rows[y].size())
            && rows[y][x] != ' ';
    };

    enum class Dir { Up = -2, Right = -1, Left = 1, Down = 2 };

    size_t steps = 0;
    struct Info {
        int x;
        int y;
        Dir dir;
    };
    auto start_x = static_cast<int>(std::distance(rows[0].begin(), std::find(rows[0].begin(), rows[0].end(), '|')));
    std::deque<Info> stack {{start_x, 0, Dir::Down}};
    std::string letters;
    while(!stack.empty()) {
        auto i = stack.back(); stack.pop_back();
        auto x = i.x;
        auto y = i.y;
        auto dir = i.dir;
        steps++;
        if(!valid_coord(x, y))   {
            steps--;
            continue;
        }

        //for plotting
        //std::cout << x << "," << rows.size() - y << std::endl;
        auto continue_dir = [&stack] (int x, int y, Dir dir) {
            switch(dir) {
                case Dir::Up: stack.push_back({x, y-1, dir}); break;
                case Dir::Down: stack.push_back({x, y+1, dir}); break;
                case Dir::Left: stack.push_back({x-1, y, dir}); break;
                case Dir::Right: stack.push_back({x+1, y, dir}); break;
            }
        };

        auto handle_crossroads = [&stack, &continue_dir] (int x, int y, Dir dir) {
            std::set<Dir> dirs = {Dir::Up, Dir::Down, Dir::Right, Dir::Left};
            dirs.erase(static_cast<Dir>(-1*static_cast<int>(dir)));
            for(auto d : dirs) {
                continue_dir(x, y, d);
            }
        };
        switch(rows[y][x]) {
            case '|': 
            case '-': 
                continue_dir(x, y, dir); break;
            case '+': 
                handle_crossroads(x, y, dir); break;
            default: 
                letters += rows[y][x];
                continue_dir(x, y, dir);
                break;
        }
    }
    return std::make_pair(steps, letters);
}

int main(int, char**) {
    Rows rows;
    for(std::string line; std::getline(std::cin, line);) rows.push_back(line);
    auto p = dfs(rows);
    std::cout << p.first << std::endl;
    //17872
    std::cout << p.second << std::endl;
    //MKXOIHZNBL
}

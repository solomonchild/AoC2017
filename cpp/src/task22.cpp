#include <iostream>
#include <vector>
#include <string>
#include <iterator>

enum class Direction {
    Up, Right, Down, Left
};

const static char CLEAN = '.';
const static char INFECTED = '#';
struct Vec2 {
    int x;
    int y;

    Vec2(int x, int y) 
        :x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const {
        return {x+other.x, y+other.y};
    }

    bool operator==(const Vec2& other) const {
        return x==other.x && y==other.y;
    }

};

struct Carrier {
    Vec2 pos;
    Direction dir;

    Carrier(const Vec2& start)
    : pos(start) { }

    void turn(Direction new_dir) {
        int cur_dir = static_cast<int>(dir);
        if(new_dir == Direction::Right)
            dir = static_cast<Direction>((cur_dir + 1)%4);
        else if(new_dir == Direction::Left)
            dir = static_cast<Direction>((cur_dir - 1 + 4)%4);
    }
};

struct Map {
    using Tile = char;
    using Row = std::string;
    using Rows = std::vector<Row>;

    Rows rows;
    Carrier& carrier;
    Map(Rows&& rows, Carrier& carrier) 
        : rows(rows),
          carrier(carrier) { }

    void expand(int how_many = 1) {
        while(how_many--) {
            for(auto& r : rows) {
                r.insert(r.begin(), CLEAN);
                r.insert(std::prev(r.end()), CLEAN);
            }
            rows.insert(rows.begin(), Row(rows.size(), CLEAN));
            rows.insert(std::prev(rows.end()), Row(rows.size(), CLEAN));
            carrier.pos.x += how_many;
            carrier.pos.y += how_many;
        }
    }
    void print() {
        auto match_x = [this](size_t j) {return j == static_cast<size_t>(carrier.pos.x);};
        for(size_t i = 0; i < rows.size(); i++) {
            if(i == static_cast<size_t>(carrier.pos.y)) {
                for(size_t j = 0; j < rows[i].size(); j++) {
                    if(match_x(j)) {
                        std::cout << "[";
                    } else if(!j && !match_x(j+1)) std::cout << " ";

                    std::cout << rows[i][j];

                    if(match_x(j)) {
                        std::cout << "]";
                    } else if(!j && !match_x(j+1)) std::cout << " ";
                }
                std::cout << std::endl;
            } else {
                std::ostream_iterator<char> oss_it(std::cout, " ");
                std::copy(rows[i].begin(), rows[i].end(), oss_it);
                std::cout << std::endl;
            }
        }
    }
};

int main(int, char**) {
    Map::Rows rows;
    for(std::string line; std::getline(std::cin, line); ) {
        rows.push_back(line);
    }
    Carrier carrier({1,1});
    Map map(std::move(rows), carrier);
    map.print();

}
#include <iostream>
#include <vector>
#include <string>
#include <iterator>

enum class Direction {
    Up, Right, Down, Left
};

const static char CLEAN = '.';
const static char INFECTED = '#';
const static char WEAKENED = 'W';
const static char FLAGGED = 'F';
struct Vec2 {
    int x;
    int y;

    Vec2(int x, int y) 
        :x(x), y(y) {}

    bool operator<(const Vec2& other) const {
        return x < other.x && y < other.y;
    }

    bool operator>(const Vec2& other) const {
        return x > other.x && y > other.y;
    }

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
        : pos(start), dir(Direction::Up) { }

    void move_forward() {
        switch(dir) {
            case Direction::Up:
                pos.y--; break;
            case Direction::Down:
                pos.y++; break;
            case Direction::Left:
                pos.x--; break;
            case Direction::Right:
                pos.x++; break;
        }

    }

    void reverse() {
        if(dir == Direction::Right) dir = Direction::Left;
        else if(dir == Direction::Left) dir = Direction::Right;
        else if(dir == Direction::Up) dir = Direction::Down;
        else if(dir == Direction::Down) dir = Direction::Up;
    }
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
    size_t size;
    Map(Rows&& rows, Carrier& carrier) 
        : rows(rows),
        carrier(carrier),
        size(rows.size()) { }

    bool expand_needed() const {
        size_t sx = static_cast<size_t>(carrier.pos.x);
        size_t sy = static_cast<size_t>(carrier.pos.y);
        return  sx == size - 1 || sx == 0
            || sy == size - 1 || sy == 0;
    }

    void expand(int how_many = 1) {
        carrier.pos.x += how_many;
        carrier.pos.y += how_many;
        while(how_many--) {
            size+=2;
            for(auto& r : rows) {
                r.insert(r.begin(), CLEAN);
                r.insert(r.end(), CLEAN);
            }
            rows.insert(rows.begin(), Row(size, CLEAN));
            rows.insert(rows.end(), Row(size, CLEAN));
        }
    }

    Tile get_tile(const Vec2& pos) const {
        if(pos < Vec2(size, size)) {
            return rows[pos.y][pos.x];
        }
        return false;
    }

    void weaken(const Vec2& pos) {
        if(pos < Vec2(size, size)) {
            rows[pos.y][pos.x] = WEAKENED;
        }
    }
    void infect(const Vec2& pos) {
        if(pos < Vec2(size, size)) {
            rows[pos.y][pos.x] = INFECTED;
        }
    }
    void flag(const Vec2& pos) {
        if(pos < Vec2(size, size)) {
            rows[pos.y][pos.x] = FLAGGED;
        }
    }
    void clean(const Vec2& pos) {
        if(pos < Vec2(size, size)) {
            rows[pos.y][pos.x] = CLEAN;
        }
    }


    void print() const {
        auto match_x = [this](size_t j) {return j == static_cast<size_t>(carrier.pos.x);};
        for(size_t i = 0; i < rows.size(); i++) {
            if(i == static_cast<size_t>(carrier.pos.y)) {
                for(size_t j = 0; j < rows[i].size(); j++) {
                    if(match_x(j)) {
                        std::cout << "[";
                    } else if(j && !match_x(j-1)) std::cout << " ";

                    std::cout << rows[i][j];

                    if(match_x(j)) {
                        std::cout << "]";
                    } 
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
    auto c = rows.size()/2;
    Carrier carrier(Vec2(c,c));
    Map map(std::move(rows), carrier);
    map.expand(3);

    map.print();
    std::cout << std::endl;
    bool part1 = true;
    size_t iter = part1 ? 10000 : 10000000;
    size_t bursts = 0;
    while(iter--) {
        if(part1) {
            if(map.get_tile(carrier.pos) != CLEAN) {
                carrier.turn(Direction::Right);
                map.clean(carrier.pos);
            } else {  
                carrier.turn(Direction::Left);
                map.infect(carrier.pos);
                bursts++;
            }
        } else {
            auto tile = map.get_tile(carrier.pos);
            if(tile == CLEAN) {
                carrier.turn(Direction::Left);
                map.weaken(carrier.pos);
            } else if(tile == WEAKENED) {
                map.infect(carrier.pos);
                bursts++;
            } else if(tile == INFECTED) {
                carrier.turn(Direction::Right);
                map.flag(carrier.pos);
            } else if(tile == FLAGGED) {
                carrier.reverse();
                map.clean(carrier.pos);
            }
        }
        carrier.move_forward();
        if(map.expand_needed()) {
            map.expand(1);
        }
    }
    map.print();
    std::cout << bursts;
    //5450 and 2511957
}

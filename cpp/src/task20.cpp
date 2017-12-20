#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cassert>

struct Vec3 {
    int64_t x;
    int64_t y;
    int64_t z;

    Vec3(int64_t x, int64_t y, int64_t z) 
        : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    Vec3& operator+=(const Vec3& other) {
        *this = Vec3{x + other.x, y + other.y, z + other.z};
        return *this;
    }
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    size_t len() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};

struct Particle {
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    Particle(const Vec3& pos, const Vec3& vel, const Vec3& acc) 
        : pos(pos),
          vel(vel),
          acc(acc)
    {}

    Particle pos_if(int ticks) {
        Particle p(*this);
        while(ticks--) {
            p.vel += p.acc;
            p.pos += p.vel;
        }
        return p;
    }

    int64_t manhattan_dist() {
        return std::abs(pos.x) + std::abs(pos.y) + std::abs(pos.z);
    }
};
template<typename T>
void print(const T& what, bool newline = true);

template<>
void print<Vec3>(const Vec3& what, bool newline)
{
    std::cout << "[" << what.x << "," << what.y << "," << what.z << "]";
    if(newline)
        std::cout << std::endl;
}

template<>
void print<Particle>(const Particle& what, bool newline)
{
    std::cout << "p=";
    print(what.pos, false);
    std::cout << ", v=";
    print(what.vel, false);
    std::cout << ", a=";
    print(what.acc, false);
    if(newline)
        std::cout << std::endl;
}

int main(int, char**) {
    using Particles = std::vector<Particle>;
    Particles particles;
    for(std::string line; std::getline(std::cin, line);) {
        std::istringstream iss(line);
        auto parse_next_vec3 = [&iss]() {
            int64_t x;
            int64_t y;
            int64_t z;
            while(!std::isdigit(iss.peek()) && iss.peek() != '-') 
                iss.ignore(1);
            iss >> x;
            iss.ignore(1);
            iss >> y;
            iss.ignore(1);
            iss >> z;
            return Vec3{x, y, z};
        };
        auto pos = parse_next_vec3();
        auto vel = parse_next_vec3();
        auto acc = parse_next_vec3();

        particles.emplace_back(pos, vel, acc);
    }
    auto it = std::min_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle& p2){
            auto l1 = p1.acc.len();
            auto l2 = p2.acc.len();
            if(l1 == l2) 
                return p1.pos.len() < p2.pos.len();
            return l1 < l2;
    });
    assert(it != particles.end());
    std::cout << std::distance(particles.begin(), it) << std::endl;
    //91
    it = std::max_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle& p2){
            return p1.acc.len() < p2.acc.len();
    });
    assert(it != particles.end());
    auto max = it->acc.len();
    std::vector<bool> destroyed(particles.size(), false);
    for(size_t i = 0; i < particles.size(); i++ ){
        if(destroyed[i]) continue;
        for(size_t j = i + 1; j < particles.size(); j++){
            if(destroyed[j]) continue;
            for(size_t t = 0; t < max; t++) {
                if(particles[i].pos_if(t).pos == particles[j].pos_if(t).pos) {
                    destroyed[i] = true;
                    destroyed[j] = true;
                    break;
                }
            }
        }
    }
    std::cout << particles.size() - std::count(destroyed.begin(), destroyed.end(), true) << std::endl;
    //567
}

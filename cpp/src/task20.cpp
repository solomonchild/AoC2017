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

    void tick(int how_many = 1) {
        while(how_many--) {
            vel += acc;
            pos += vel;
        }
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
            return p1.acc.len() < p2.acc.len();
    });
    assert(it != particles.end());
    std::cout << std::distance(particles.begin(), it) << std::endl;
}

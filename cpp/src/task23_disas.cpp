#include <iostream>

void unused() {
    int64_t b = 109300;
    const int64_t c = 126300;
    int64_t h = 0;
    int64_t f = 0;
    int64_t d = 0;
    do {
        f = 1;
        d = 2;
        do {
            int e = 2;
            do {
                if(d*e == b)
                    f = 0;
                e++;
            } while(e != b); //mark 3
            d++;
        } while(d != b); //mark4

        if(f == 0)
            h++;
        if(b == c)
            b += 17;
        std::cout << h<< std::endl;
    } while(b != c);
    std::cout << h << std::endl;
}

void part1() {

}
int main() {
    part1();

}

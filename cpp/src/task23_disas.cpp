#include <iostream>
int main() {
    size_t b = 109300;
    size_t c = 111000;
    int f = 1;
    int d = 2;
    int h = 0;
    int g = 0;
    do {
    do {
        int e = 2;
        do {
            g = d * e;
            g -= b;
            if(g == 0)
                f = 0;
            e += 1;
            g = e;
            g -= b;
        } while(g == 0); //mark 4
        d++;
        g = d - b;
    } while(true); //mark5
    if(f==0)
        h++;
    g = b - c;
    if(g == 0)
        b += 17;
    } while(g == 0);
    std::cout << h << std::endl;
}

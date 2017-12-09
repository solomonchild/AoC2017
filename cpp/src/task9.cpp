#include <iostream>
#include <iterator>
#include <utility>
#include <numeric>

int main() {
    enum class State{ 
        Reading,
        Skip,
        Comment,
    };

    using Tuple = std::pair<size_t, size_t>;
    Tuple t = std::accumulate(std::istream_iterator<char>(std::cin), {}, Tuple(), [level = 0, state = State::Reading] (const Tuple& tuple, char c) mutable {
        Tuple ret = tuple;
        switch(state) {
            case State::Skip: {
              state = State::Comment;
            } break;
            case State::Reading: {
                if(c == '{') {
                    level++;
                }		
                else if(c == '}') {
                    ret = std::make_pair(tuple.first + level, tuple.second);
                    level--;
                }
                else if(c == '<') {
                    state = State::Comment;
                }
            } break;
            case State::Comment: {
                if(c == '>') state = State::Reading;
                else if(c == '!') state = State::Skip;
                else ret = std::make_pair(tuple.first, tuple.second + 1);
            } break;
        }
        return ret;
    });
    std::cout << t.first << std::endl;
    //11347
    std::cout << t.second << std::endl;
    //5404
}

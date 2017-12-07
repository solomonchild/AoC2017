set NUM=%1
g++ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM% --std=c++11 && task%NUM%.exe < task%NUM%.txt

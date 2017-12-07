@echo off

set NUM=%1
if exist task%NUM%.txt (
    g++ -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM% --std=c++11 && task%NUM%.exe < task%NUM%.txt
) else (
    g++ -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM% --std=c++11 && task%NUM%.exe 
)

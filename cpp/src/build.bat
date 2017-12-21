@echo off
cls

set NUM=%1
if exist task%NUM%.txt (
    clang++ -Xclang -flto-visibility-public-std -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM%.exe --std=c++14 && task%NUM%.exe < task%NUM%.txt
) else (
    clang++ -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM%.exe --std=c++14 && task%NUM%.exe 
)

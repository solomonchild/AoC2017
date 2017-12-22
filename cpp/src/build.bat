@echo off
cls

set NUM=%1
set COMP=%2
IF "%COMP%"=="mingw" (
set CC=g++
set FLAGS=
) ELSE (
set CC=clang++
set FLAGS=-Xclang -flto-visibility-public-std
)
if exist task%NUM%.txt (
    %CC% %FLAGS% -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM%.exe --std=c++14 && task%NUM%.exe < task%NUM%.txt
) else (
    %CC% -I..\inc\ -Wall -Wextra -Werror -pedantic task%NUM%.cpp -o task%NUM%.exe --std=c++14 && task%NUM%.exe 
) 

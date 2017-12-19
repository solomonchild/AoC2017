rem write to txt file to process later
del infi.exe && clang++ infi.cpp --std=c++14 -o infi.exe &&  infi.exe < infi.txt > out.txt
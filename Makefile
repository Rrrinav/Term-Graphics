cc:= g++
flags:= -ggdb 

main: main.cpp
	$(cc) main.cpp -o main $(flags) && ./main

debug: main.cpp
	$(cc) main.cpp -o main $(flags) && gdb ./main

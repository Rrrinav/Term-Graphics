cc:= g++
flags:= -ggdb 

main: main.cpp
	$(cc) main.cpp -o main $(flags) && ./main

debug: main.cpp
	$(cc) main.cpp -o main $(flags) && gdb ./main

color: color.cpp
	$(cc) color.cpp -o color $(flags) && ./color

example_1: Examples/sum_of_sines.cpp
	$(cc) Examples/sum_of_sines.cpp -o example1 $(flags) && ./example1

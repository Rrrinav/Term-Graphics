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

engine: sprite_engine.cpp
	$(cc) sprite_engine.cpp -o engine $(flags) && ./engine

example_2: Examples/wall.cpp
	cd Examples && $(cc) wall.cpp -o example2 $(flags) && example2

example_3: Examples/perlin_noise.cpp
	cd Examples && $(cc) perlin_noise.cpp -o example3 $(flags) && example3

example_4: Examples/perlin_noise.cpp
	cd Examples && $(cc) gradient_and_button.cpp -o example3 $(flags) && example3

cc := g++
flags := -ggdb -Wall -Wextra
build_dir := build

# Create the build directory if it doesn't exist
$(shell mkdir -p $(build_dir))

# Target for the main executable
main: main.cpp
	$(cc) main.cpp -o $(build_dir)/main $(flags) && ./$(build_dir)/main

# Target for debugging
debug: main.cpp
	$(cc) main.cpp -o $(build_dir)/main $(flags) && gdb ./$(build_dir)/main

# Target for the color executable
color: color.cpp
	$(cc) color.cpp -o $(build_dir)/color $(flags) && ./$(build_dir)/color

# Example 1: sum of sines
example_1: Examples/1sum_of_sines.cpp
	$(cc) Examples/1sum_of_sines.cpp -o $(build_dir)/example1 $(flags) && ./$(build_dir)/example1

# Target for the sprite engine executable
engine: sprite_engine.cpp
	$(cc) sprite_engine.cpp -o $(build_dir)/engine $(flags) && ./$(build_dir)/engine

# Example 2: wall
example_2: Examples/2wall.cpp
	$(cc) Examples/2wall.cpp -o $(build_dir)/example2 $(flags) && ./$(build_dir)/example2

# Example 3: perlin noise
example_3: Examples/3perlin_noise.cpp
	$(cc) Examples/3perlin_noise.cpp -o $(build_dir)/example3 $(flags) && ./$(build_dir)/example3

# Example 4: gradient and button
example_4: Examples/4gradient_and_button.cpp
	$(cc) Examples/4gradient_and_button.cpp -o $(build_dir)/example4 $(flags) && ./$(build_dir)/example4

# Example 4: gradient and button
example_5: Examples/5ball.cpp
	$(cc) Examples/5ball.cpp -o $(build_dir)/example5 $(flags) && ./$(build_dir)/example5

main3: main3.cpp
	$(cc) main3.cpp -o $(build_dir)/main3 $(flags) && ./$(build_dir)/main3

# Clean up build directory
clean:
	rm -rf $(build_dir)/*

.PHONY: clean

#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "../dependencies/color.hpp"

class Sprite
{
    size_t _width;
    size_t _height;
    std::vector<char> _characters;
    std::vector<Color> _colors;

public:
    Sprite() = default;
    Sprite(const std::string &filename) { load_from_file(filename); }
    Sprite(size_t width, size_t height, std::vector<char> characters, std::vector<Color> colors)
        : _width(width), _height(height), _characters(characters), _colors(colors)
    {
    }

    // Format of the file:
    // it will be a txt file with the following format:
    // width height
    // character_1 color_1 (Color will be in 32 bit hex format 0xrrggbb)
    // character_2 color_2
    // ...
    void load_from_file(const std::string &filename)
    {
        // Skip comments or empty lines
        _characters.clear();
        _colors.clear();

        std::ifstream file(filename);
        if (!file.is_open())
            std::cerr << "Error: Unable to open sprite file: " + filename << std::endl;

        if (!(file >> _width >> _height))
            std::cerr << "Error: Unable to read sprite dimensions from file: " + filename << std::endl;
        
        
        _characters.resize(_width * _height, ' ');
        _colors.resize(_width * _height, Color());  // Initialize with default Color

        // Read the characters and colors
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore rest of the line after reading dimensions

        size_t index = 0;
        std::string line;
        while (std::getline(file, line) && index < _width * _height)
        {
            if (line.empty())
            {
                _characters[index] = ' ';
                _colors[index] = Color();  // Default color
            }
            else
            {
                std::istringstream iss(line);
                char character;
                std::string hex_color_str;

                if (!(iss >> std::noskipws >> character))
                {
                    std::cerr << "Error: Unable to read character at index: " << index << std::endl;
                    break;
                }

                // Read the rest of the line for color, if present
                if (iss >> std::ws && std::getline(iss, hex_color_str))
                {
                    uint32_t hex_color;
                    try
                    {
                        hex_color = std::stoul(hex_color_str, nullptr, 16);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error: Invalid color format at index: " << index << " (" << hex_color_str << ")" << std::endl;
                        break;
                    }
                    _colors[index] = Color(hex_color);
                }
                else
                {
                    std::cerr << "Error: Missing color for character at index: " << index << std::endl;
                    break;
                }

                _characters[index] = character;
            }
            index++;
        }

        if (index != _width * _height)
            std::cerr << "Error: Data read mismatch with sprite dimensions." << std::endl;
        file.close();
    }

    void save_to_file(const std::string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open sprite file for writing: " + filename << std::endl;
            return;
        }

        file << _width << " " << _height << "\n";
        for (size_t i = 0; i < _characters.size(); ++i)
        {
            char character = _characters[i];
            uint32_t color = Color::to_hex(_colors[i]);

            // Format color to a 6-digit hex string
            file << character << " " << std::setw(6) << std::setfill('0') << std::hex << color << "\n";
        }

        file.close();
    }
    size_t width() const { return _width; }
    size_t height() const { return _height; }

    std::vector<char> characters() const { return _characters; }
    std::vector<Color> colors() const { return _colors; }
    void set_colors(const std::vector<Color> &colors) { _colors = colors; }
};

#pragma once

#include <cmath>
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
  Sprite(size_t width, size_t height) : _width(width), _height(height)
  {
    _characters.resize(_width * _height, ' ');
    _colors.resize(_width * _height, Color());
  }

  char get_char_n(float x, float y) const
  {
    // Clamp x and y to the [0, 1] range and scale to the sprite dimensions
    x = std::fmod(std::fmax(0.0f, x), 1.0f);  // Wrap around if x exceeds 1.0
    y = std::fmod(std::fmax(0.0f, y), 1.0f);  // Wrap around if y exceeds 1.0

    // Map normalized x, y to the respective indices
    size_t nx = static_cast<size_t>(x * _width);
    size_t ny = static_cast<size_t>(y * _height);

    return _characters[ny * _width + nx];
  }

  char get_char_un(float x, float y, size_t tiling_factor_width, size_t tiling_factor_height) const
  {
    float normalized_x = (static_cast<float>(x) / tiling_factor_width);
    float normalized_y = (static_cast<float>(y) / tiling_factor_height);

    // Wrap coordinates to repeat sprite
    normalized_x = std::fmod(normalized_x, 1.0f);  // Keeps value between 0 and 1
    normalized_y = std::fmod(normalized_y, 1.0f);

    // Pass wrapped normalized coordinates to get_char and get_color
    return get_char_n(normalized_x, normalized_y);
  }

  Color get_color_n(float x, float y) const
  {
    // Same normalization as get_char for x and y
    x = std::fmod(std::fmax(0.0f, x), 1.0f);  // Wrap around if x exceeds 1.0
    y = std::fmod(std::fmax(0.0f, y), 1.0f);  // Wrap around if y exceeds 1.0

    // Map normalized x, y to the respective indices
    size_t nx = static_cast<size_t>(x * _width);
    size_t ny = static_cast<size_t>(y * _height);

    return _colors[ny * _width + nx];
  }

  Color get_color_un(float x, float y, size_t tiling_factor_width, size_t tiling_factor_height) const
  {
    float normalized_x = (static_cast<float>(x) / tiling_factor_width);
    float normalized_y = (static_cast<float>(y) / tiling_factor_height);

    // Wrap coordinates to repeat sprite
    normalized_x = std::fmod(normalized_x, 1.0f);  // Keeps value between 0 and 1
    normalized_y = std::fmod(normalized_y, 1.0f);

    // Pass wrapped normalized coordinates to get_char and get_color
    return get_color_n(normalized_x, normalized_y);
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

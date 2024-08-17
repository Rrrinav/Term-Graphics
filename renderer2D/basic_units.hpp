#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>

#define L_GEBRA_IMPLEMENTATION
#include "../dependencies/color.hpp"
#include "../l_gebra/l_gebra.hpp"  // Assuming this is your external library header

// Pixel class represents a pixel in the buffer with two characters and two colors
class Pixel
{
public:
  char _ch1;              // First character for the pixel
  char _ch2;              // Second character for the pixel (for double-pixeling)
  Color _color1;          // Color for the first character
  Color _color2;          // Color for the second character
  bool _is_empty = true;  // Flag to check if the pixel is considered "empty"

  // Default constructor
  Pixel() = default;

  // Constructor with a single character and color
  // @param ch The character to set for both _ch1 and _ch2
  // @param color The color to set for both _color1 and _color2
  Pixel(char ch, Color color) : _ch1(ch), _ch2(ch), _color1(color), _color2(color), _is_empty(ch == ' ') {}

  // Constructor with two characters and a single color
  // @param ch1 The first character
  // @param ch2 The second character
  // @param color The color to set for both _color1 and _color2
  Pixel(char ch1, char ch2, Color color) : _ch1(ch1), _ch2(ch2), _color1(color), _color2(color), _is_empty(ch1 == ' ' && ch2 == ' ') {}

  // Constructor with two characters and two colors
  // @param ch1 The first character
  // @param ch2 The second character
  // @param color1 The color for the first character
  // @param color2 The color for the second character
  Pixel(char ch1, char ch2, Color color1, Color color2)
      : _ch1(ch1), _ch2(ch2), _color1(color1), _color2(color2), _is_empty(ch1 == ' ' && ch2 == ' ')
  {
  }

  // Destructor
  ~Pixel() = default;

  // Set both colors to the same value
  // @param color The color to set for both _color1 and _color2
  void set_color(Color color)
  {
    _color1 = color;
    _color2 = color;
  }

  // Set both characters to the same value
  // @param ch The character to set for both _ch1 and _ch2
  void set_char(char ch)
  {
    _ch1 = ch;
    _ch2 = ch;
    _is_empty = (ch == ' ');
  }

  // Set both characters with different values
  // @param ch1 The first character
  // @param ch2 The second character
  void set_char(char ch1, char ch2)
  {
    _ch1 = ch1;
    _ch2 = ch2;
    _is_empty = (ch1 == ' ' && ch2 == ' ');
  }

  // Set both characters and both colors
  // @param ch The character to set for both _ch1 and _ch2
  // @param color The color to set for both _color1 and _color2
  void set(char ch, Color color)
  {
    _ch1 = ch;
    _ch2 = ch;
    _color1 = color;
    _color2 = color;
    _is_empty = (ch == ' ');
  }

  // Set characters and color
  // @param ch1 The first character
  // @param ch2 The second character
  // @param color The color to set for both _color1 and _color2
  void set(char ch1, char ch2, Color color)
  {
    _ch1 = ch1;
    _ch2 = ch2;
    _color1 = color;
    _color2 = color;
    _is_empty = (ch1 == ' ' && ch2 == ' ');
  }

  // Set the second color only
  // @param color The color to set for _color2
  void set_color2(Color color) { _color2 = color; }

  // Check if the pixel is empty
  // @return true if both characters are spaces, false otherwise
  bool is_empty() const { return _is_empty; }
};

// Buffer class represents a 2D buffer of Pixels
class Buffer
{
public:
  std::unique_ptr<Pixel[]> data;  // Unique pointer to an array of Pixels
  size_t width;                   // Width of the buffer
  size_t height;                  // Height of the buffer

  // Default constructor initializes with no data
  Buffer() : data(nullptr), width(0), height(0) {}

  // Constructor with width and height, initializes all pixels as empty
  // @param width Width of the buffer
  // @param height Height of the buffer
  Buffer(size_t width, size_t height) : data(std::make_unique<Pixel[]>(width * height)), width(width), height(height)
  {
    for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(' ', Color());
  }

  // Constructor with width, height, fill character, and color
  // @param width Width of the buffer
  // @param height Height of the buffer
  // @param fill The character to fill the buffer with
  // @param color The color to use for all pixels
  Buffer(size_t width, size_t height, char fill, Color color)
      : data(std::make_unique<Pixel[]>(width * height)), width(width), height(height)
  {
    for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(fill, color);
  }

  // Copy constructor
  // @param other The buffer to copy from
  Buffer(const Buffer &other) : data(std::make_unique<Pixel[]>(other.width * other.height)), width(other.width), height(other.height)
  {
    std::copy(other.data.get(), other.data.get() + width * height, data.get());
  }

  // Copy assignment operator
  // @param other The buffer to copy from
  // @return A reference to the modified buffer
  Buffer &operator=(const Buffer &other)
  {
    if (this != &other)
    {
      width = other.width;
      height = other.height;
      data = std::make_unique<Pixel[]>(width * height);
      std::copy(other.data.get(), other.data.get() + width * height, data.get());
    }
    return *this;
  }

  // Set a pixel in the buffer at a specific point
  // @param point The position to set the pixel
  // @param ch The character for the pixel
  // @param color The color for the pixel
  void set(utl::Vec<int, 2> point, char ch, Color color)
  {
    int x = point.x();
    int y = point.y();
    if (x >= 0 && static_cast<size_t>(x) < width && y >= 0 && static_cast<size_t>(y) < height)
      data[y * width + x].set(ch, color);
  }

  // Set a pixel with two characters and a single color
  // @param point The position to set the pixel
  // @param ch1 The first character for the pixel
  // @param ch2 The second character for the pixel
  // @param color The color for the pixel
  void set(utl::Vec<int, 2> point, char ch1, char ch2, Color color)
  {
    int x = point.x();
    int y = point.y();
    if (x >= 0 && static_cast<size_t>(x) < width && y >= 0 && static_cast<size_t>(y) < height)
      data[y * width + x] = Pixel(ch1, ch2, color);
  }

  // Set a pixel with two characters and two colors
  // @param point The position to set the pixel
  // @param ch1 The first character for the pixel
  // @param ch2 The second character for the pixel
  // @param color1 The first color for the pixel
  // @param color2 The second color for the pixel
  void set(utl::Vec<int, 2> point, char ch1, char ch2, Color color1, Color color2)
  {
    int x = point.x();
    int y = point.y();
    if (x >= 0 && static_cast<size_t>(x) < width && y >= 0 && static_cast<size_t>(y) < height)
      data[y * width + x] = Pixel(ch1, ch2, color1, color2);
  }

  void set_absolute(utl::Vec<int, 2> point, char ch, bool left, Color color)
  {
    int x = point.x();
    int y = point.y();
    if (x >= 0 && static_cast<size_t>(x) < width && y >= 0 && static_cast<size_t>(y) < height)
    {
      if (left)
      {
        data[y * width + x]._ch1 = ch;
        data[y * width + x]._color1 = color;
      }
      else
      {
        data[y * width + x]._ch2 = ch;
        data[y * width + x]._color2 = color;
      }
    }
  }

  // Access a pixel using (x, y) coordinates
  // @param x The x-coordinate of the pixel
  // @param y The y-coordinate of the pixel
  // @return A reference to the Pixel at (x, y)
  Pixel &operator()(size_t x, size_t y) { return data[y * width + x]; }

  // Access a pixel using (x, y) coordinates (const version)
  // @param x The x-coordinate of the pixel
  // @param y The y-coordinate of the pixel
  // @return A const reference to the Pixel at (x, y)
  const Pixel &operator()(size_t x, size_t y) const { return data[y * width + x]; }

  // Fill the entire buffer with a character and color
  // @param ch The character to fill the buffer with
  // @param color The color to use for all pixels
  void fill(char ch, Color color)
  {
    for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(ch, color);
  }

  // Clear the buffer (set all pixels to empty)
  void clear()
  {
    for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(' ', Color());
  }
};

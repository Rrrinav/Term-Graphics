
#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>
#include <string>

#include "./color.hpp"
#include "./sprites.hpp"
#include "stb_image.h"

namespace utl
{

  class Image
  {
    int _width;
    int _height;
    int _channels;
    unsigned char *_pixels;

  public:
  public:
    Image(const std::string &filePath) { loadImage(filePath); }

    ~Image() { stbi_image_free(_pixels); }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    int getChannels() const { return _channels; }
    const unsigned char *getPixels() const { return _pixels; }

    char pixelToChar(int x, int y) const
    {
      int index = (y * _width + x) * _channels;
      unsigned char r = _pixels[index];
      unsigned char g = _pixels[index + 1];
      unsigned char b = _pixels[index + 2];

      // Calculate brightness
      float brightness = 0.299f * r + 0.587f * g + 0.114f * b;

      // Normalize brightness to a value between 0 and 1
      float normalizedBrightness = brightness / 255.0f;

      // Map normalized brightness to a character
      int charIndex = std::round(normalizedBrightness * (char_gradient.size() - 1));

      return char_gradient[charIndex];
    }

    Color getColor(int x, int y) const
    {
      int index = (y * _width + x) * _channels;
      unsigned char r = _pixels[index];
      unsigned char g = _pixels[index + 1];
      unsigned char b = _pixels[index + 2];
      return Color(r, g, b);
    }

    Sprite convert_image_to_sprite()
    {
      size_t width = _width;
      size_t height = _height;

      std::vector<char> characters(width * height);
      std::vector<Color> colors(width * height);

      for (size_t y = 0; y < height; ++y)
      {
        for (size_t x = 0; x < width; ++x)
        {
          // Convert pixel to brightness
          char ascii_char = pixelToChar(x, y);

          // Get the original color (or grayscale it)
          Color color = getColor(x, y);

          // Store character and color
          characters[y * width + x] = ascii_char;
          colors[y * width + x] = color;  // or grayscale version of the color
        }
      } 
      return Sprite(width, height, characters, colors);
    }

  private:
    void loadImage(const std::string &filePath)
    {
      // Load image data
      _pixels = stbi_load(filePath.c_str(), &_width, &_height, &_channels, 0);
      if (!_pixels)
        throw std::runtime_error("Failed to load image: " + filePath);
    }
  };

}  // namespace utl

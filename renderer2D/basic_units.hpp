#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>

#define L_GEBRA_IMPLEMENTATION
#include "../dependencies/color.hpp"
#include "../l_gebra/l_gebra.hpp"  // Assuming this is your external library header

class Pixel
{
public:
    char _ch1;
    char _ch2;
    Color _color1;
    Color _color2;
    bool _is_empty = true;

    Pixel() = default;
    Pixel(char ch, Color color) : _ch1(ch), _ch2(ch), _color1(color), _color2(color)
    {
        if (ch == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    Pixel(char ch1, char ch2, Color color) : _ch1(ch1), _ch2(ch2), _color1(color), _color2(color)
    {
        if (ch1 == ' ' && ch2 == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    Pixel(char ch1, char ch2, Color color1, Color color2) : _ch1(ch1), _ch2(ch2), _color1(color1), _color2(color2)
    {
      if (ch1 == ' ' && ch2 == ' ')
          _is_empty = true;
      else
          _is_empty = false;
    }
    ~Pixel() = default;
    void set_color(Color color)
    {
        _color1 = color;
        _color2 = color;
    }
    void set_char(char ch)
    {
        _ch1 = ch;
        _ch2 = ch;
        if (ch == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    void set_char(char ch1, char ch2)
    {
        _ch1 = ch1;
        _ch2 = ch2;
        if (ch1 == ' ' && ch2 == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    void set(char ch, Color color)
    {
        _ch1 = ch;
        _ch2 = ch;
        _color1 = color;
        _color2 = color;
        if (ch == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    void set(char ch1, char ch2, Color color)
    {
        _ch1 = ch1;
        _ch2 = ch2;
        _color1 = color;
        _color2 = color;
        if (ch1 == ' ' && ch2 == ' ')
            _is_empty = true;
        else
            _is_empty = false;
    }
    void set_color2(Color color) { _color2 = color; }
    bool is_empty() { return _is_empty; }
};

// Buffer class
class Buffer
{
public:
    std::unique_ptr<Pixel[]> data;
    size_t width;
    size_t height;

    Buffer() : data(nullptr), width(0), height(0) {}

    Buffer(size_t width, size_t height) : data(std::make_unique<Pixel[]>(width * height)), width(width), height(height)
    {
        for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(' ', Color());
    }

    Buffer(size_t width, size_t height, char fill, Color color)
        : data(std::make_unique<Pixel[]>(width * height)), width(width), height(height)
    {
        for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(fill, color);
    }

    Buffer(const Buffer &other) : data(std::make_unique<Pixel[]>(other.width * other.height)), width(other.width), height(other.height)
    {
        std::copy(other.data.get(), other.data.get() + width * height, data.get());
    }

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

    void set(utl::Vec<int, 2> point, char ch, Color color)
    {
        size_t x = point.x();
        size_t y = point.y();
        if (x >= 0 && x < width && y >= 0 && y < height)
            data[y * width + x] = Pixel(ch, color);
    }
    void set(utl::Vec<int, 2> point, char ch1, char ch2, Color color)
    {
        size_t x = point.x();
        size_t y = point.y();
        if (x >= 0 && x < width && y >= 0 && y < height)
            data[y * width + x] = Pixel(ch1, ch2, color);
    }
    void set(utl::Vec<int, 2> point, char ch1, char ch2, Color color1, Color color2)
    {
        size_t x = point.x();
        size_t y = point.y();
        if (x >= 0 && x < width && y >= 0 && y < height)
            data[y * width + x] = Pixel(ch1, ch2, color1, color2);
    }
    Pixel &operator()(size_t x, size_t y) { return data[y * width + x]; }
    const Pixel &operator()(size_t x, size_t y) const { return data[y * width + x]; }
    void fill(char ch, Color color)
    {
        for (size_t i = 0; i < width * height; ++i) data[i] = Pixel(ch, color);
    }
};

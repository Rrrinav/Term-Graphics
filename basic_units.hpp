#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>

#define L_GEBRA_IMPLEMENTATION
#include "./l_gebra.hpp"  // Assuming this is your external library header
#include "color.hpp"

class Pixel
{
public:
    char _ch;
    Color _color;

    Pixel() = default;
    Pixel(char ch, Color color) : _ch(ch), _color(color) {}
    ~Pixel() = default;
    void set_color(Color color) { _color = color; }
    void set_char(char ch) { _ch = ch; }
    void set(char ch, Color color)
    {
        _ch = ch;
        _color = color;
    }
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
        for (size_t i = 0; i < width * height; ++i)
        {
            data[i] = Pixel(' ', Color());
        }
    }

    Buffer(size_t width, size_t height, char fill, Color color)
        : data(std::make_unique<Pixel[]>(width * height)), width(width), height(height)
    {
        for (size_t i = 0; i < width * height; ++i)
        {
            data[i] = Pixel(fill, color);
        }
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
        {
            data[y * width + x] = Pixel(ch, color);
        }
    }

    Pixel &operator()(size_t x, size_t y) { return data[y * width + x]; }
    const Pixel &operator()(size_t x, size_t y) const { return data[y * width + x]; }
    void fill(char ch, Color color)
    {
        for (size_t i = 0; i < width * height; ++i)
        {
            data[i] = Pixel(ch, color);
        }
    }
};

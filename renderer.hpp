#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// PERF: We need to make this faster, super faster

#define L_GEBRA_IMPLEMENTATION
#include "./l_gebra.hpp"  // Assuming this is your external library header
#include "basic_units.hpp"
#include "color.hpp"
#include "font.hpp"
#include "sprites.hpp"
#include "window.hpp"
//Anti-aliasing will depend on if it top of pixel or bottom of pixel too
static char anti_aliasing[2][2] = {{'`', '^'}, {'_', 'a'}};

// Renderer class
class Renderer
{
    std::shared_ptr<Buffer> _buffer;
    Color _bg_color = Color(TRANSPARENT);
    Pixel *_pixels;
    Window _window;

public:
    Renderer();
    Renderer(size_t width, size_t height);
    Renderer(std::shared_ptr<Buffer> buffer);

    const Buffer &get_buffer() const;
    size_t get_width() const;
    size_t get_height() const;
    void Init();

    bool draw_point(utl::Vec<int, 2> point, char c, Color color = Color(WHITE));
    bool draw_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color = Color(WHITE));
    bool draw_anti_aliased_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color = Color(WHITE));
    bool draw_xaolin_wu_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color = Color(WHITE));
    bool draw_circle(utl::Vec<int, 2> center, int radius, char ch, Color color = WHITE);
    bool draw_fill_circle(utl::Vec<int, 2> center, int radius, char ch, Color color = WHITE);
    bool draw_rectangle(utl::Vec<int, 2> start, int width, int height, char ch, char char2 = '@', Color color = WHITE);
    bool draw_fill_rectangle(utl::Vec<int, 2> start, int width, int height, char ch, Color color = WHITE);
    bool draw_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    bool draw_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    bool draw_xaolin_wu_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    bool draw_fill_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    bool draw_fill_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    bool draw_polygon(std::vector<utl::Vec<int, 2>> vertices, char ch, Color color = WHITE);
    bool draw_arc(utl::Vec<int, 2> center, int radius, char ch, float end_angle, float start_angle = 0.0f, Color color = WHITE);
    bool draw_text(utl::Vec<int, 2> start, const std::string &text, Color color = WHITE);
    bool draw_text_with_font(utl::Vec<int, 2> start, const std::string &text, Color color, const Font &font);
    bool draw_text_with_shadow(utl::Vec<int, 2> start, const std::string &text, Color color, Color shadow_color, const Font &font,
                               int shadow_offset_x = 1, int shadow_offset_y = 1);
    static Font load_font(const std::string &font_path);
    Sprite load_sprite(const std::string &sprite_path);
    bool draw_sprite(utl::Vec<int, 2> start, const Sprite &sprite, Color color = WHITE);

    void draw();
    static std::shared_ptr<Buffer> create_buffer(size_t width, size_t height);
    void empty();
    static void clear_screen();
    static void reset_screen();
    void fill_buffer(char c, Color color);
    void set_bg_color(Color color);
    static inline void sleep(int milliseconds);
    static void move_cursor(size_t x, size_t y);
    static void hide_cursor();
    static void show_cursor();

private:
    void draw_circle_octants(const utl::Vec<int, 2> &center, int x, int y, char ch, Color color = WHITE);
};

#ifdef RENDERER_IMPLEMENTATION

#include <chrono>
#include <iostream>
#include <thread>

// Renderer definitions
Renderer::Renderer() : _buffer(std::make_shared<Buffer>()), _pixels(nullptr) {}

Renderer::Renderer(size_t width, size_t height) : _buffer(std::make_shared<Buffer>(width, height)) {}

Renderer::Renderer(std::shared_ptr<Buffer> buffer) : _buffer(buffer) {}

const Buffer &Renderer::get_buffer() const { return *_buffer; }

size_t Renderer::get_width() const { return _buffer->width; }

size_t Renderer::get_height() const { return _buffer->height / 2; }

void Renderer::Init() { _window.init_terminal(); }

bool Renderer::draw_point(utl::Vec<int, 2> point, char c, Color color)
{
    if (point.x() < 0 || point.x() >= static_cast<int>(_buffer->width) || point.y() < 0 || point.y() >= static_cast<int>(_buffer->height))
    {
        return false;
    }

    int x = point.x();
    int y = point.y();

    // Ensure y is even for rendering
    y = (y % 2 == 0) ? y : y - 1;
    y = (y < 0) ? 0 : y;

    if (x >= 0 && x < static_cast<int>(_buffer->width) && y >= 0 && y < static_cast<int>(_buffer->height))
    {
        _buffer->set({x, y}, c, color);
        return true;
    }
    return false;
}

bool Renderer::draw_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color)
{
    // Anti-aliasing will depend upon left or right of the pixel and top or bottom of the pixel
    int x1 = start[0], y1 = start[1];
    int x2 = end[0], y2 = end[1];

    // Ensure y1 and y2 are even for rendering
    y1 = (y1 % 2 == 0) ? y1 : y1 - 1;
    y2 = (y2 % 2 == 0) ? y2 : y2 - 1;

    // Calculate the difference between the points
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    // Determine the direction of the line
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;
    int e2;

    while (true)
    {
        int prev_x = x1;
        int prev_y = y1;
        // Plot the point
        if (x1 >= 0 && x1 < static_cast<int>(_buffer->width) && y1 >= 0 && y1 < static_cast<int>(_buffer->height))
        {
            _buffer->set({x1, y1}, c, color);
        }

        // Check if the end point is reached
        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }

    return true;
}

bool Renderer::draw_anti_aliased_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color)
{
    // Anti-aliasing will depend upon left or right of the pixel and top or bottom of the pixel
    int x1 = start[0], y1 = start[1];
    int x2 = end[0], y2 = end[1];

    // Ensure y1 and y2 are even for rendering
    y1 = (y1 % 2 == 0) ? y1 : y1 - 1;
    y2 = (y2 % 2 == 0) ? y2 : y2 - 1;

    // Calculate the difference between the points
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    // Determine the direction of the line
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;
    int e2;

    // case for horizontal line or vertical line
    if (dx == 0 || dy == 0)
    {
        draw_line(start, end, c, color);
        return true;
    }
    while (true)
    {
        int prev_x = x1;
        int prev_y = y1;
        // Plot the point
        if (x1 >= 0 && x1 < static_cast<int>(_buffer->width) && y1 >= 0 && y1 < static_cast<int>(_buffer->height))
        {
            // Anti-aliasing will depend upon left or right of the pixel and top or bottom of the pixel
            if (prev_x == x1)
            {
                if (prev_y < y1)
                {
                    (*_buffer).set({x1, y1}, anti_aliasing[0][0], color);
                    (*_buffer).set({x1, y1 + 1}, anti_aliasing[0][1], color);
                }
                else
                {
                    (*_buffer).set({x1, y1}, anti_aliasing[1][0], color);
                    (*_buffer).set({x1, y1 + 1}, anti_aliasing[1][1], color);
                }
            }
            else if (prev_y == y1)
            {
                if (prev_x < x1)
                {
                    (*_buffer).set({x1, y1}, anti_aliasing[0][0], color);
                    (*_buffer).set({x1 + 1, y1}, anti_aliasing[0][1], color);
                }
                else
                {
                    (*_buffer).set({x1, y1}, anti_aliasing[1][0], color);
                    (*_buffer).set({x1 + 1, y1}, anti_aliasing[1][1], color);
                }
            }
            else
            {
                _buffer->set({x1, y1}, c, color);
            }
        }

        // Check if the end point is reached
        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }

    return true;
}

bool Renderer::draw_xaolin_wu_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color)
{
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();

    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    float gradient = (dx == 0) ? 1.0f : static_cast<float>(dy) / dx;

    // Handle first endpoint
    int xend = std::round(x0);
    int yend = y0 + gradient * (xend - x0);
    float xgap = 1 - (x0 + 0.5f - std::floor(x0 + 0.5f));
    int xpxl1 = xend;  //this will be used in the main loop
    int ypxl1 = std::floor(yend);
    if (steep)
    {
        if (draw_point({ypxl1, xpxl1}, c) && draw_point({ypxl1 + 1, xpxl1}, c))
        {
            _buffer->set({ypxl1, xpxl1}, c, color);
            _buffer->set({ypxl1 + 1, xpxl1}, c, color);
        }
        xpxl1 = xend + 1;
    }
    else
    {
        if (draw_point({xpxl1, ypxl1}, c) && draw_point({xpxl1, ypxl1 + 1}, c))
        {
            _buffer->set({xpxl1, ypxl1}, c, color);
            _buffer->set({xpxl1, ypxl1 + 1}, c, color);
        }
        xpxl1 = xend + 1;
    }

    float intery = yend + gradient;  //first y-intersection for the main loop

    // Handle second endpoint
    xend = std::round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = x0 + 0.5f - std::floor(x0 + 0.5f);
    int xpxl2 = xend;  //this will be used in the main loop
    int ypxl2 = std::floor(yend);
    if (steep)
    {
        if (draw_point({ypxl2, xpxl2}, c) && draw_point({ypxl2 + 1, xpxl2}, c))
        {
            _buffer->set({ypxl2, xpxl2}, c, color);
            _buffer->set({ypxl2 + 1, xpxl2}, c, color);
        }
        xpxl2 = xend + 1;
    }
    else
    {
        if (draw_point({xpxl2, ypxl2}, c) && draw_point({xpxl2, ypxl2 + 1}, c))
        {
            _buffer->set({xpxl2, ypxl2}, c, color);
            _buffer->set({xpxl2, ypxl2 + 1}, c, color);
        }
        xpxl2 = xend + 1;
    }

    // Main loop
    for (int x = xpxl1; x < xpxl2; ++x)
    {
        if (steep)
        {
            if (draw_point({static_cast<int>(intery), x}, c) && draw_point({static_cast<int>(intery) + 1, x}, c))
            {
                float alpha = intery - std::floor(intery);
                if (alpha <= 0.5)
                {
                    _buffer->set({static_cast<int>(intery), x}, anti_aliasing[0][0], color);
                    _buffer->set({static_cast<int>(intery) + 1, x}, anti_aliasing[0][1], color);
                }
                else
                {
                    _buffer->set({static_cast<int>(intery), x}, anti_aliasing[1][0], color);
                    _buffer->set({static_cast<int>(intery) + 1, x}, anti_aliasing[1][1], color);
                }
            }
        }
        else
        {
            if (draw_point({x, static_cast<int>(intery)}, c) && draw_point({x, static_cast<int>(intery) + 1}, c))
            {
                float alpha = intery - std::floor(intery);
                if (alpha <= 0.5)
                {
                    _buffer->set({x, static_cast<int>(intery)}, anti_aliasing[0][0], color);
                    _buffer->set({x, static_cast<int>(intery) + 1}, anti_aliasing[0][1], color);
                }
                else
                {
                    _buffer->set({x, static_cast<int>(intery)}, anti_aliasing[1][0], color);
                    _buffer->set({x, static_cast<int>(intery) + 1}, anti_aliasing[1][1], color);
                }
            }
        }
        intery += gradient;  // Increment y intersection
    }

    return true;
}
bool Renderer::draw_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    draw_anti_aliased_line(a, b, ch, color);
    draw_anti_aliased_line(b, c, ch, color);
    draw_anti_aliased_line(c, a, ch, color);
    return true;
}

bool Renderer::draw_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    draw_line(a, b, ch, color);
    draw_line(b, c, ch, color);
    draw_line(c, a, ch, color);
    return true;
}
bool Renderer::draw_xaolin_wu_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    draw_xaolin_wu_line(a, b, ch, color);
    draw_xaolin_wu_line(b, c, ch, color);
    draw_xaolin_wu_line(c, a, ch, color);
    return true;
}

bool Renderer::draw_fill_circle(utl::Vec<int, 2> center, int radius, char ch, Color color)
{
    // We will implement anti-aliasing now for the circle
    utl::Vec<int, 2> begin = center + (-1 * radius);
    utl::Vec<int, 2> end = center + radius;

    if (radius % 2 == 0 && begin[1] - 1 > 0 && begin[1] - 1 < _buffer->height) begin[1] = begin[1] - 1;
    for (int y = begin.y(); y <= end.y(); ++y)
    {
        for (int x = begin.x(); x <= end.x(); ++x)
        {
            float distance = center.distance(utl::Vec<int, 2>{x, y});

            // Ensure y is even for rendering
            int render_y = (y % 2 == 0) ? y : y - 1;

            if (distance <= radius)
            {
                _buffer->set({x, render_y}, ch, color);  // Drawing character
            }  // calculate the anti-aliasing
            else if (distance <= radius + 1)
            {
                float alpha = std::abs((float)distance - radius);
                //Case 1: Top of pixel i.e. y is above the center
                if (y > center.y())
                {
                    // next criteria is coverage
                    if (alpha <= 0.5)
                    {
                        _buffer->set({x, render_y}, anti_aliasing[0][0], color);
                    }
                    else
                    {
                        _buffer->set({x, render_y}, anti_aliasing[0][1], color);
                    }
                }
                else  //Case 2: Bottom of pixel i.e. y is below the center
                {
                    if (alpha <= 0.5)
                    {
                        _buffer->set({x, render_y}, anti_aliasing[1][0], color);
                    }
                    else
                    {
                        _buffer->set({x, render_y}, anti_aliasing[1][1], color);
                    }
                }
            }
        }
    }

    return true;
}

bool Renderer::draw_circle(utl::Vec<int, 2> center, int radius, char ch, Color color)
{
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while (x >= y)
    {
        // Draw pixels in all octants
        draw_circle_octants(center, x, y, ch, color);
        y++;

        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
    return true;
}

void Renderer::draw_circle_octants(const utl::Vec<int, 2> &center, int x, int y, char ch, Color color)
{
    // Upper octants
    char old = ch;
    _buffer->set({center.x() + x, center.y() + y}, ch, color);
    _buffer->set({center.x() - x, center.y() + y}, ch, color);
    _buffer->set({center.x() + y, center.y() + x}, ch, color);
    _buffer->set({center.x() - y, center.y() + x}, ch, color);

    // Lower octants
    _buffer->set({center.x() + x, center.y() - y}, ch, color);
    _buffer->set({center.x() - x, center.y() - y}, ch, color);
    _buffer->set({center.x() + y, center.y() - x}, ch, color);
    _buffer->set({center.x() - y, center.y() - x}, ch, color);
}

bool Renderer::draw_polygon(std::vector<utl::Vec<int, 2>> vertices, char ch, Color color)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        draw_line(vertices[i], vertices[(i + 1) % vertices.size()], ch, color);
    }
    draw_line(vertices[vertices.size() - 1], vertices[0], ch, color);
    return true;
}

bool Renderer::draw_rectangle(utl::Vec<int, 2> point, int width, int height, char ch, char ch2, Color color)
{
    // top-right to bottom-right
    draw_line({point.x(), point.y() + height}, point, ch2, color);
    // bottom-left to top-left
    draw_line({point.x() + width, point.y()}, {point.x() + width, point.y() + height}, ch2, color);
    // top-left to top-right
    draw_line(point, {point.x() + width, point.y()}, ch, color);
    // bottom-right to bottom-left
    draw_line({point.x() + width, point.y() + height}, {point.x(), point.y() + height}, ch, color);
    return true;
}

bool Renderer::draw_fill_rectangle(utl::Vec<int, 2> point, int width, int height, char ch, Color color)
{
    utl::Vec<int, 2> end = point + utl::Vec<int, 2>{width, height};
    for (int y = point.y(); y <= end.y(); ++y)
    {
        for (int x = point.x(); x <= end.x(); ++x)
        {
            // Ensure y is even for rendering
            if (x >= 0 && x < static_cast<int>(_buffer->width) && y >= 0 && y < static_cast<int>(_buffer->height))
            {
                _buffer->set({x, y}, ch, color);
            }
        }
    }
    return true;
}

bool Renderer::draw_fill_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    // Sort the vertices by y-coordinate
    if (a.y() > b.y()) std::swap(a, b);
    if (a.y() > c.y()) std::swap(a, c);
    if (b.y() > c.y()) std::swap(b, c);
    // Function to draw a horizontal line within bounds
    auto safe_draw_line = [&](int x1, int y, int x2)
    {
        if (y < 0 || y >= static_cast<int>(_buffer->height)) return;
        if (x1 > x2) std::swap(x1, x2);
        if (x2 < 0 || x1 >= static_cast<int>(_buffer->width)) return;
        x1 = std::max(x1, 0);
        x2 = std::min(x2, static_cast<int>(_buffer->width) - 1);
        for (int x = x1; x <= x2; ++x)
        {
            _buffer->set({x, y}, ch, color);
        }
    };
    // Function to interpolate x between two points at a given y
    auto interpolate_x = [](utl::Vec<int, 2> p1, utl::Vec<int, 2> p2, int y) -> int
    {
        if (p1.y() == p2.y()) return p1.x();  // Horizontal line
        return p1.x() + (y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
    };
    // Fill the upper part of the triangle (from a to b)
    for (int y = a.y(); y <= b.y(); ++y)
    {
        int x1 = interpolate_x(a, b, y);
        int x2 = interpolate_x(a, c, y);
        safe_draw_line(x1, y, x2);
    }
    // Fill the lower part of the triangle (from b to c)
    for (int y = b.y(); y <= c.y(); ++y)
    {
        int x1 = interpolate_x(b, c, y);
        int x2 = interpolate_x(a, c, y);
        safe_draw_line(x1, y, x2);
    }
    return true;
}

bool Renderer::draw_fill_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    // Sort the vertices by y-coordinate
    if (a.y() > b.y()) std::swap(a, b);
    if (a.y() > c.y()) std::swap(a, c);
    if (b.y() > c.y()) std::swap(b, c);

    // Function to draw a horizontal line within bounds
    auto safe_draw_line = [&](int x1, int y, int x2)
    {
        if (y < 0 || y >= static_cast<int>(_buffer->height)) return;
        if (x1 > x2) std::swap(x1, x2);
        if (x2 < 0 || x1 >= static_cast<int>(_buffer->width)) return;
        x1 = std::max(x1, 0);
        x2 = std::min(x2, static_cast<int>(_buffer->width) - 1);
        for (int x = x1; x <= x2; ++x)
        {
            _buffer->set({x, y}, ch, color);
        }
    };

    // Function to interpolate x between two points at a given y
    auto interpolate_x = [](utl::Vec<int, 2> p1, utl::Vec<int, 2> p2, int y) -> int
    {
        if (p1.y() == p2.y()) return p1.x();  // Horizontal line
        return p1.x() + (y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
    };

    int prev_x1 = 0;
    int prev_x2 = 0;
    int count = 0;
    // Fill the upper part of the triangle (from a to b)
    for (int y = a.y(); y <= b.y(); ++y)
    {
        int x1 = interpolate_x(a, b, y);
        int x2 = interpolate_x(a, c, y);
        safe_draw_line(x1, y, x2);
        // Implement anti-aliasing here based on number of empty pixels
        if (count && prev_x1 != x1)
        {
            draw_anti_aliased_line({prev_x1, y - 1}, {x1, y}, ch);
        }
        if (count && prev_x2 != x2)
        {
            draw_anti_aliased_line({prev_x2, y - 1}, {x2, y}, ch);
        }
        prev_x1 = x1;
        prev_x2 = x2;
        count++;
    }
    count = 0;
    // Fill the lower part of the triangle (from b to c)
    for (int y = b.y(); y <= c.y(); ++y)
    {
        int x1 = interpolate_x(b, c, y);
        int x2 = interpolate_x(a, c, y);
        safe_draw_line(x1, y, x2);
        if (count && prev_x1 != x1)
        {
            draw_anti_aliased_line({prev_x1, y - 1}, {x1, y}, ch);
        }
        if (count && prev_x2 != x2)
        {
            draw_anti_aliased_line({prev_x2, y - 1}, {x2, y}, ch);
        }
        prev_x1 = x1;
        prev_x2 = x2;
        count++;
    }

    return true;
}

bool Renderer::draw_arc(utl::Vec<int, 2> center, int radius, char ch, float end_angle, float start_angle /* = 0.0f */,
                        Color color /* = WHITE */)
{
    float step = 1.0f / radius;  // Step size for angle increment
    float angle = start_angle;

    while (angle <= end_angle)
    {
        int x = static_cast<int>(center.x() + radius * std::cos(angle));
        int y = static_cast<int>(center.y() - radius * std::sin(angle));
        _buffer->set({x, y}, ch, color);

        if (y <= radius + 1)
        {
            float alpha = std::abs((float)y - radius);
            //Case 1: Top of pixel i.e. y is above the center
            if (y > center.y())
            {
                // next criteria is coverage
                if (alpha <= 0.5)
                {
                    _buffer->set({x, y}, anti_aliasing[0][0], color);
                }
                else
                {
                    _buffer->set({x, y}, anti_aliasing[0][1], color);
                }
            }
            else  //Case 2: Bottom of pixel i.e. y is below the center
            {
                if (alpha <= 0.5)
                {
                    _buffer->set({x, y}, anti_aliasing[1][0], color);
                }
                else
                {
                    _buffer->set({x, y}, anti_aliasing[1][1], color);
                }
            }
        }

        angle += step;
    }

    return true;
}

bool Renderer::draw_text(utl::Vec<int, 2> start, const std::string &text, Color color)
{
    int x = start.x();
    int y = start.y();
    for (size_t i = 0; i < text.size(); ++i)
    {
        if (x >= 0 && x < static_cast<int>(_buffer->width) && y >= 0 && y < static_cast<int>(_buffer->height))
        {
            _buffer->set({x, y}, text[i], color);
        }
        x++;
    }
    return true;
}

bool Renderer::draw_text_with_font(utl::Vec<int, 2> start, const std::string &text, Color color, const Font &font)
{
    int x = start.x();
    int y = start.y();
    int px = 0;
    int py = 0;
    int count = 0;
    for (char ch : text)
    {
        if (font.get_glyph(ch).is_empty()) continue;
        const Glyph &glyph = font.get_glyph(ch);
        const std::vector<std::string> &lines = glyph.get_lines();
        // Render each line of the glyph
        for (size_t j = 0; j < lines.size(); j++)
        {
            for (size_t k = 0; k < lines[j].size(); ++k)
            {
                px = x + k;
                py = y + j * 2;
                _buffer->set({px, py}, lines[j][k], color);
            }
        }
        x += glyph.get_width() + 1;
        if (x + glyph.get_width() + 1 >= static_cast<int>(_buffer->width))
        {
            x = start.x();
            y += (glyph.get_height() + 1) * 2;
        }
    }

    return true;
}

bool Renderer::draw_text_with_shadow(utl::Vec<int, 2> start, const std::string &text, Color color, Color shadow_color, const Font &font,
                                     int shadow_offset_x /* = 1 */, int shadow_offset_y /* = 1 */)
{
    int x = start.x();
    int y = start.y();

    int px = 0;
    int py = 0;
    // Draw the shadow first
    for (char ch : text)
    {
        if (font.get_glyph(ch).is_empty()) continue;
        const Glyph &glyph = font.get_glyph(ch);
        const std::vector<std::string> &lines = glyph.get_lines();

        for (size_t j = 0; j < lines.size(); j++)
        {
            for (size_t k = 0; k < lines[j].size(); ++k)
            {
                px = x + k + shadow_offset_x;
                py = y + j * 2 + shadow_offset_y * 2;

                _buffer->set({px, py}, lines[j][k], shadow_color);
            }
        }
        x += glyph.get_width() + 1 + ((shadow_offset_x > 0) ? shadow_offset_x : shadow_offset_x * -1);
        if (x >= static_cast<int>(_buffer->width))
        {
            x = start.x();
            y += (glyph.get_height() + 1 + shadow_offset_y) * 2;
        }
    }

    // Reset position to draw the actual text
    x = start.x();
    y = start.y();

    // Draw the actual text
    for (char ch : text)
    {
        if (font.get_glyph(ch).is_empty()) continue;
        const Glyph &glyph = font.get_glyph(ch);
        const std::vector<std::string> &lines = glyph.get_lines();
        // Render each line of the glyph
        for (size_t j = 0; j < lines.size(); j++)
        {
            for (size_t k = 0; k < lines[j].size(); ++k)
            {
                px = x + k;
                py = y + j * 2;
                if ((*_buffer)(px, py)._ch != ' ' && lines[j][k] == ' ') continue;
                _buffer->set({px, py}, lines[j][k], color);
            }
        }
        x += glyph.get_width() + 1 + ((shadow_offset_x > 0) ? shadow_offset_x : shadow_offset_x * -1);
        ;
        if (x >= static_cast<int>(_buffer->width))
        {
            x = start.x();
            y += (glyph.get_height() + 1 + shadow_offset_y) * 2;
        }
    }

    return true;
}

Font Renderer::load_font(const std::string &font_path)
{
    Font font;
    if (font.load_from_file(font_path))
    {
        return font;
    }
    return Font();
}

void Renderer::draw()
{
    std::string print_buffer;

    if (_bg_color != Color(TRANSPARENT))
    {
        print_buffer += _bg_color.to_ansii_bg_str();
        // std::cout << _bg_color.to_ansii_bg_str();
    }
    for (size_t y = 0; y < _buffer->height; y += 2)
    {
        for (size_t x = 0; x < _buffer->width; x++)
        {
            // Implement anti-aliasing when drawing the buffer too
            //std::cout << (*_buffer)(x, y)._color.to_ansii_fg_str() << (*_buffer)(x, y)._ch;
            print_buffer += (*_buffer)(x, y)._color.to_ansii_fg_str();
            print_buffer += (*_buffer)(x, y)._ch;
        }
        // // fprintf(stdout, "%.*s\n", static_cast<int>(_buffer->width), print);
        // std::cout.write(print, _buffer->width);
        //std::cout << '\n';
        print_buffer += '\n';
    }
    //    std::cout << ANSII_BG_RESET;
    print_buffer += ANSII_BG_RESET;
    _window.draw(print_buffer);
}

Sprite Renderer::load_sprite(const std::string &sprite_path)
{
    Sprite sprite;
    if (sprite.load_from_file(sprite_path))
    {
        return sprite;
    }
    return Sprite();
}

bool Renderer::draw_sprite(utl::Vec<int, 2> start_pos, const Sprite &sprite, Color color)
{
    auto lines = sprite.get_data();
    int px = 0;
    int py = 0;
    int x = start_pos.x();
    int y = start_pos.y();
    // for (auto &row : data)
    // {
    //     for (auto &ch : row)
    //     {
    //       std::cout << ch;
    //     }
    //     std::cout << '\n';
    //
    // }
    for (size_t j = 0; j < lines.size(); j++)
    {
        for (size_t k = 0; k < lines[j].size(); ++k)
        {
            px = x + k;
            py = y + j * 2;
            _buffer->set({px, py}, lines[j][k], color);
        }
    }
    return true;
}

std::shared_ptr<Buffer> Renderer::create_buffer(size_t width, size_t height) { return std::make_shared<Buffer>(width, height); }

void Renderer::empty() { _buffer->fill(' ', Color()); }

void Renderer::clear_screen() { std::cout << "\033[2J"; }

void Renderer::reset_screen()
{
    clear_screen();
    std::cout << "\033[0;0H";
}

void Renderer::fill_buffer(char c, Color color) { _buffer->fill(c, color); }
void Renderer::set_bg_color(Color color) { this->_bg_color = color; }
void Renderer::sleep(int milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }

void Renderer::move_cursor(size_t x, size_t y) { std::cout << "\033[" << y << ";" << x << "H"; }

void Renderer::hide_cursor() { std::cout << "\033[?25l"; }

void Renderer::show_cursor() { std::cout << "\033[?25h"; }

// TODO: Frame rate control, line clipping, Texture Mapping, double buffering
//       Camera system, merging buffers, gradient fill, 3D rendering, sprite rendering, user input
//       windowing system

#endif  // RENDERER_IMPLEMENTATION

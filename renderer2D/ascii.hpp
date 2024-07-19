#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// PERF: We need to make this faster, super faster
// TODO: Testing on windows
// BUG: Anti-aliasing in circle sucks
// TODO: More and better error handling and exception handling (even though we dont need much of it)

#define L_GEBRA_IMPLEMENTATION

#include "../dependencies/color.hpp"
#include "../dependencies/font.hpp"
#include "../dependencies/glyph.hpp"
#include "../dependencies/gradient.hpp"
#include "../dependencies/shapes.hpp"
#include "../dependencies/sprites.hpp"
#include "../l_gebra/l_gebra.hpp"
#include "../window/window.hpp"
#include "basic_units.hpp"

//Anti-aliasing will depend on if it top of pixel or bottom of pixel too
static char anti_aliasing[2][2] = {{'`', '^'}, {'-', 'c'}};

// Renderer class
class Renderer
{
    std::shared_ptr<Buffer> _buffer;
    Color _bg_color = Color(TRANSPARENT);
    Window _window;

public:
    Renderer();
    Renderer(size_t width, size_t height);
    Renderer(std::shared_ptr<Buffer> buffer);
    Renderer(size_t width, size_t height, Color bg_color) : _buffer(std::make_shared<Buffer>(width, height)), _bg_color(bg_color)
    {
        Init();
    }
    ~Renderer() { _window.cleanup_terminal(); }

    const Buffer &get_buffer() const;
    size_t get_width() const;
    size_t get_height() const;
    void Init();
    void end() { _window.cleanup_terminal(); }
    // TODO: Implement draw methods using shapes and caching for all the shapes bruv..
    bool draw_point(utl::Vec<int, 2> point, char c, Color color = Color(WHITE));
    bool draw_point2(utl::Vec<int, 2> point, char c, char c2, Color color = Color(WHITE));
    bool draw_point(const Point &point)
    {
        bool i = draw_point2(point.get_pos(), point.get_char(), point.get_char2(), point.get_color());
        return i;
    }
    bool draw_half_point(utl::Vec<int, 2> point, char c, bool left, Color color = Color(WHITE))
    {
        _buffer->set_absolute(point, c, left, color);
        return true;
    }
    bool draw_half_point(const half_point &point)
    {
        _buffer->set_absolute(point.get_pos(), point.get_char(), point.is_left(), point.get_color());
        return true;
    }
    void draw_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color = Color(WHITE));
    void draw_line(const Line &line) { draw_line(line.get_start(), line.get_end(), line.get_char(), line.get_color()); }
    void draw_anti_aliased_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color = Color(WHITE));
    void draw_anti_aliased_line(const Line &line)
    {
        draw_anti_aliased_line(line.get_start(), line.get_end(), line.get_char(), line.get_color());
    }
    void draw_circle(utl::Vec<int, 2> center, int radius, char ch, Color color = WHITE);
    void draw_circle(const Circle &circle) { draw_circle(circle.get_center(), circle.get_radius(), circle.get_char(), circle.get_color()); }
    void draw_fill_circle(utl::Vec<int, 2> center, int radius, char ch, Color color = WHITE);
    void draw_fill_circle(const Circle &circle)
    {
        draw_fill_circle(circle.get_center(), circle.get_radius(), circle.get_char(), circle.get_color());
    }
    void draw_rectangle(utl::Vec<int, 2> start, int width, int height, char ch, char char2 = '@', Color color = WHITE);
    void draw_rectangle(const Rectangle &rectangle)
    {
        draw_rectangle(
            rectangle.get_top_left(), rectangle.get_width(), rectangle.get_height(), rectangle.get_char(), '@', rectangle.get_color());
    }
    void draw_fill_rectangle(utl::Vec<int, 2> start, int width, int height, char ch, Color color = WHITE);
    void draw_fill_rectangle(const Rectangle &rectangle)
    {
        draw_fill_rectangle(
            rectangle.get_top_left(), rectangle.get_width(), rectangle.get_height(), rectangle.get_char(), rectangle.get_color());
    }
    void draw_rect_linear_gradient(utl::Vec<int, 2> start, int width, int height, char ch, Gradient &gradient, bool horizontal = true)
    {
        for (int i = 0; i < (horizontal ? width : height); i++)
        {
            float t = static_cast<float>(i) / (horizontal ? width : height);
            Color color = gradient.get_color_at(t);
            for (int j = 0; j < (horizontal ? height : width); j++)
                if (horizontal)
                    _buffer->set({start.x() + i, start.y() + j}, ch, color);
                else
                    _buffer->set({start.x() + j, start.y() + i}, ch, color);
        }
    }

    // Utility function to rotate coordinates
    std::pair<float, float> rotate_point(float x, float y, float angle)
    {
        float cos_theta = std::cos(angle);
        float sin_theta = std::sin(angle);
        float rotated_x = x * cos_theta - y * sin_theta;
        float rotated_y = x * sin_theta + y * cos_theta;
        return {rotated_x, rotated_y};
    }

    void draw_rect_rotated_gradient(utl::Vec<int, 2> start, int width, int height, char ch, Gradient &gradient, float angle)
    {
        // Convert angle to radians

        // Determine the direction of the gradient
        float gradient_direction_x = std::cos(angle);
        float gradient_direction_y = std::sin(angle);
        float max_distance = std::sqrt(width * width + height * height);
        // Iterate over each pixel in the rectangle
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                // Calculate the pixel position relative to the start of the rectangle
                float px = start.x() + i;
                float py = start.y() + j;

                // Rotate the pixel position to align with the gradient direction
                auto [rotated_x, rotated_y] = rotate_point(px - start.x() - width / 2.0f, py - start.y() - height / 2.0f, -angle);

                // Map rotated coordinates to gradient progress
                float t = (rotated_x + width / 2.0f) / width;
                // Get the color at the normalized position
                Color color = gradient.get_color_at(t);

                // Set the pixel in the buffer
                _buffer->set({start.x() + i, start.y() + j}, ch, color);
            }
        }
    }
    void draw_rect_radial_gradient(utl::Vec<int, 2> start, int width, int height, char ch, Gradient &gradient)
    {
        float half_width = width / 2.0f;
        float half_height = height / 2.0f;
        // Calculate the center of the rectangle
        float centerX = start.x() + half_width;
        float centerY = start.y() + half_height;

        float max_distance = std::sqrt(half_width * half_width + half_height * half_height);
        // Iterate over each pixel in the rectangle
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                // Calculate the distance of the current pixel from the center
                float distanceX = start.x() + i - centerX;
                float distanceY = start.y() + j - centerY;
                float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                // Normalize the distance to get a value between 0 and 1
                float t = distance / max_distance;

                // Get the color at the normalized distance
                Color color = gradient.get_color_at(t);

                // Set the pixel in the buffer
                _buffer->set({start.x() + i, start.y() + j}, ch, color);
            }
        }
    }
    void draw_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    void draw_triangle(const Triangle &triangle)
    {
        auto points = triangle.get_vertices();
        draw_triangle(points[0], points[1], points[2], triangle.get_char(), triangle.get_color());
    }
    void draw_antialiased_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    void draw_antialiased_triangle(const Triangle &triangle)
    {
        auto points = triangle.get_vertices();
        draw_antialiased_triangle(points[0], points[1], points[2], triangle.get_char(), triangle.get_color());
    }
    void draw_xaolin_wu_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    void draw_xaolin_wu_triangle(const Triangle &triangle)
    {
        auto points = triangle.get_vertices();
        draw_xaolin_wu_triangle(points[0], points[1], points[2], triangle.get_char(), triangle.get_color());
    }
    void draw_fill_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    void draw_fill_triangle(const Triangle &triangle)
    {
        auto points = triangle.get_vertices();
        draw_fill_triangle(points[0], points[1], points[2], triangle.get_char(), triangle.get_color());
    }
    void draw_fill_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color = WHITE);
    void draw_fill_antialias_triangle(const Triangle &triangle)
    {
        auto points = triangle.get_vertices();
        draw_fill_antialias_triangle(points[0], points[1], points[2], triangle.get_char(), triangle.get_color());
    }
    void draw_polygon(std::vector<utl::Vec<int, 2>> vertices, char ch, Color color = WHITE);
    void draw_polygon(const Polygon &polygon) { draw_polygon(polygon.get_vertices(), polygon.get_char(), polygon.get_color()); }
    void draw_arc(utl::Vec<int, 2> center, int radius, char ch, float end_angle, float start_angle = 0.0f, Color color = WHITE);
    void draw_text(utl::Vec<int, 2> start, const std::string &text, Color color = WHITE);
    void draw_text_with_font(utl::Vec<int, 2> start, const std::string &text, Color color, const Font &font);
    void draw_text_with_shadow(utl::Vec<int, 2> start, const std::string &text, Color color, Color shadow_color, const Font &font,
                               int shadow_offset_x = 1, int shadow_offset_y = 1);
    static Font load_font(const std::string &font_path);
    Glyph load_glyph(const std::string &glyph_path);
    void draw_glyph(utl::Vec<int, 2> start_pos, const Glyph &glyph, Color color = WHITE);
    void draw_sprite(const utl::Vec<int, 2> start_pos, const Sprite &sprite)
    {
        auto characters = sprite.characters();
        auto colors = sprite.colors();
        size_t width = sprite.width();
        int x1 = 0, x2 = 0;
        for (size_t y = 0; y < sprite.height(); y++)
            for (size_t x = 0; x < sprite.width() / 2; x++)
            {
                x1 = 2 * x;
                x2 = 2 * x + 1;
                _buffer->set({(int)(start_pos.x() + x), (int)(start_pos.y() + y)},
                             characters[y * width + x1],
                             characters[y * width + x2],
                             colors[y * width + x1],
                             colors[y * width + x2]);
            }
        if (sprite.width() % 2)
            for (size_t y = 0; y < sprite.height(); y++)
                _buffer->set({(int)(start_pos.x() + x2), (int)(start_pos.y() + y)},
                             characters[y * width + (width - 1)],
                             ' ',
                             colors[y * width + width - 1],
                             Color());
    }
    void sync_fps() {}
    void print();
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
    void draw_circle_octants(const utl::Vec<int, 2> &center, int x, int y, char ch, Color color);
};

#ifdef RENDERER_IMPLEMENTATION

#include <chrono>
#include <iostream>
#include <thread>

// Renderer definitions
Renderer::Renderer() : _buffer(std::make_shared<Buffer>()) {}

Renderer::Renderer(size_t width, size_t height) : _buffer(std::make_shared<Buffer>(width, height)) { Init(); }

Renderer::Renderer(std::shared_ptr<Buffer> buffer) : _buffer(buffer) { Init(); }

const Buffer &Renderer::get_buffer() const { return *_buffer; }

size_t Renderer::get_width() const { return _buffer->width; }

size_t Renderer::get_height() const { return _buffer->height / 2; }

void Renderer::Init() { _window.init_terminal(); }

bool Renderer::draw_point(utl::Vec<int, 2> point, char c, Color color)
{
    if (point.x() < 0 || point.x() >= static_cast<int>(_buffer->width) || point.y() < 0 || point.y() >= static_cast<int>(_buffer->height))
        return false;

    int x = point.x();
    int y = point.y();

    _buffer->set({x, y}, c, color);
    return true;

    return false;
}

bool Renderer::draw_point2(utl::Vec<int, 2> point, char c, char c2, Color color)
{
    int x = point.x();
    int y = point.y();

    if (x >= 0 && x < static_cast<int>(_buffer->width) && y >= 0 && y < static_cast<int>(_buffer->height))
        _buffer->set({x, y}, c, c2, color);

    return true;
}

void Renderer::draw_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color)
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
            _buffer->set({x1, y1}, c, color);

        // Check if the end point is reached
        if (x1 == x2 && y1 == y2)
            break;

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
}

void Renderer::draw_anti_aliased_line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char c, Color color)
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

    // Characters of different densities
    const char density_chars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

    auto blend_color = [](const Color &c1, const Color &c2, float t) -> Color
    {
        return Color(static_cast<uint8_t>(c1.r() * (1 - t) + c2.r() * t),
                     static_cast<uint8_t>(c1.g() * (1 - t) + c2.g() * t),
                     static_cast<uint8_t>(c1.b() * (1 - t) + c2.b() * t));
    };

    // Handle first endpoint
    int xend = std::round(x0);
    int yend = y0 + gradient * (xend - x0);
    float xgap = 1 - (x0 + 0.5f - std::floor(x0 + 0.5f));
    int xpxl1 = xend;  // this will be used in the main loop
    int ypxl1 = std::floor(yend);
    if (steep)
    {
        _buffer->set(
            {ypxl1, xpxl1}, density_chars[int((1.0f - (yend - ypxl1) * xgap) * 10)], blend_color(color, Color(), (yend - ypxl1) * xgap));
        _buffer->set(
            {ypxl1 + 1, xpxl1}, density_chars[int(((yend - ypxl1) * xgap) * 10)], blend_color(color, Color(), 1 - (yend - ypxl1) * xgap));
    }
    else
    {
        _buffer->set(
            {xpxl1, ypxl1}, density_chars[int((1.0f - (yend - ypxl1) * xgap) * 10)], blend_color(color, Color(), (yend - ypxl1) * xgap));
        _buffer->set(
            {xpxl1, ypxl1 + 1}, density_chars[int(((yend - ypxl1) * xgap) * 10)], blend_color(color, Color(), 1 - (yend - ypxl1) * xgap));
    }

    float intery = yend + gradient;  // first y-intersection for the main loop

    // Handle second endpoint
    xend = std::round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = x0 + 0.5f - std::floor(x0 + 0.5f);
    int xpxl2 = xend;  // this will be used in the main loop
    int ypxl2 = std::floor(yend);
    if (steep)
    {
        _buffer->set(
            {ypxl2, xpxl2}, density_chars[int((1.0f - (yend - ypxl2) * xgap) * 10)], blend_color(color, Color(), (yend - ypxl2) * xgap));
        _buffer->set(
            {ypxl2 + 1, xpxl2}, density_chars[int(((yend - ypxl2) * xgap) * 10)], blend_color(color, Color(), 1 - (yend - ypxl2) * xgap));
    }
    else
    {
        _buffer->set(
            {xpxl2, ypxl2}, density_chars[int((1.0f - (yend - ypxl2) * xgap) * 10)], blend_color(color, Color(), (yend - ypxl2) * xgap));
        _buffer->set(
            {xpxl2, ypxl2 + 1}, density_chars[int(((yend - ypxl2) * xgap) * 10)], blend_color(color, Color(), 1 - (yend - ypxl2) * xgap));
    }

    // Main loop
    for (int x = xpxl1; x < xpxl2; ++x)
    {
        if (steep)
        {
            float alpha = intery - std::floor(intery);
            _buffer->set({static_cast<int>(intery), x}, density_chars[int((1.0f - alpha) * 10)], blend_color(color, Color(), alpha));
            _buffer->set({static_cast<int>(intery) + 1, x}, density_chars[int(alpha * 10)], blend_color(color, Color(), 1 - alpha));
        }
        else
        {
            float alpha = intery - std::floor(intery);
            _buffer->set({x, static_cast<int>(intery)}, density_chars[int((1.0f - alpha) * 10)], blend_color(color, Color(), alpha));
            _buffer->set({x, static_cast<int>(intery) + 1}, density_chars[int(alpha * 10)], blend_color(color, Color(), 1 - alpha));
        }
        intery += gradient;  // Increment y intersection
    }
}

void Renderer::draw_antialiased_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    draw_anti_aliased_line(a, b, ch, color);
    draw_anti_aliased_line(b, c, ch, color);
    draw_anti_aliased_line(c, a, ch, color);
}

void Renderer::draw_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    draw_line(a, b, ch, color);
    draw_line(b, c, ch, color);
    draw_line(c, a, ch, color);
}

void Renderer::draw_fill_circle(utl::Vec<int, 2> center, int radius, char ch, Color color)
{
    // Define the anti-aliasing shades
    char anti_aliasing[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    int shades = sizeof(anti_aliasing) / sizeof(anti_aliasing[0]);

    // Define the start and end points for the drawing loop
    utl::Vec<int, 2> begin = center + (-1 * radius);
    utl::Vec<int, 2> end = center + radius;

    // Loop through the pixels in the bounding box
    for (int y = begin.y(); y <= end.y(); ++y)
    {
        for (int x = begin.x(); x <= end.x(); ++x)
        {
            float distance = center.distance(utl::Vec<int, 2>{x, y});

            if (distance <= radius)
            {
                // Fully inside the circle, draw the character with full opacity
                _buffer->set({x, y}, ch, color);
            }
            else if (distance < radius + 1)
            {
                // On the boundary, calculate the coverage for anti-aliasing
                float alpha = radius + 1 - distance;
                int shade_index = std::min((int)(alpha * shades), shades - 1);
                char shade_char = anti_aliasing[shade_index];
                _buffer->set({x, y}, shade_char, color);
            }
        }
    }
}

void Renderer::draw_circle(utl::Vec<int, 2> center, int radius, char ch, Color color)
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
}

void Renderer::draw_circle_octants(const utl::Vec<int, 2> &center, int x, int y, char ch, Color color)
{
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

void Renderer::draw_polygon(std::vector<utl::Vec<int, 2>> vertices, char ch, Color color)
{
    for (size_t i = 0; i < vertices.size(); i++) draw_line(vertices[i], vertices[(i + 1) % vertices.size()], ch, color);
    draw_line(vertices[vertices.size() - 1], vertices[0], ch, color);
}

void Renderer::draw_rectangle(utl::Vec<int, 2> point, int width, int height, char ch, char ch2, Color color)
{
    // top-right to bottom-right
    draw_line({point.x(), point.y() + height}, point, ch2, color);
    // bottom-left to top-left
    draw_line({point.x() + width, point.y()}, {point.x() + width, point.y() + height}, ch2, color);
    // top-left to top-right
    draw_line(point, {point.x() + width, point.y()}, ch, color);
    // bottom-right to bottom-left
    draw_line({point.x() + width, point.y() + height}, {point.x(), point.y() + height}, ch, color);
}

void Renderer::draw_fill_rectangle(utl::Vec<int, 2> point, int width, int height, char ch, Color color)
{
    utl::Vec<int, 2> end = point + utl::Vec<int, 2>{width, height};
    for (int y = point.y(); y <= end.y(); ++y)
        for (int x = point.x(); x <= end.x(); ++x) _buffer->set({x, y}, ch, color);
}

void Renderer::draw_fill_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    // Sort the vertices by y-coordinate
    if (a.y() > b.y())
        std::swap(a, b);
    if (a.y() > c.y())
        std::swap(a, c);
    if (b.y() > c.y())
        std::swap(b, c);
    // Function to draw a horizontal line within bounds
    auto safe_draw_line = [&](int x1, int y, int x2)
    {
        // if (y < 0 || y >= static_cast<int>(_buffer->height))
        //     return;
        // if (x1 > x2)
        //     std::swap(x1, x2);
        // if (x2 < 0 || x1 >= static_cast<int>(_buffer->width))
        //     return;
        // x1 = std::max(x1, 0);
        // x2 = std::min(x2, static_cast<int>(_buffer->width) - 1);
        for (int x = x1; x <= x2; ++x) _buffer->set({x, y}, ch, color);
    };
    // Function to interpolate x between two points at a given y
    auto interpolate_x = [](utl::Vec<int, 2> p1, utl::Vec<int, 2> p2, int y) -> int
    {
        if (p1.y() == p2.y())
            return p1.x();  // Horizontal line
        return p1.x() + (y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
    };
    // Fill the upper part of the triangle (from a to b)
    for (int y = a.y(); y < b.y(); ++y)
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
}

void Renderer::draw_fill_antialias_triangle(utl::Vec<int, 2> a, utl::Vec<int, 2> b, utl::Vec<int, 2> c, char ch, Color color)
{
    // Sort the vertices by y-coordinate
    if (a.y() > b.y())
        std::swap(a, b);
    if (a.y() > c.y())
        std::swap(a, c);
    if (b.y() > c.y())
        std::swap(b, c);

    // Function to draw a horizontal line within bounds
    auto safe_draw_line = [&](int x1, int y, int x2)
    {
        // if (y < 0 || y >= static_cast<int>(_buffer->height))
        //     return;
        // if (x1 > x2)
        //     std::swap(x1, x2);
        // if (x2 < 0 || x1 >= static_cast<int>(_buffer->width))
        //     return;
        // x1 = std::max(x1, 0);
        // x2 = std::min(x2, static_cast<int>(_buffer->width) - 1);
        for (int x = x1; x <= x2; ++x) _buffer->set({x, y}, ch, color);
    };

    // Function to interpolate x between two points at a given y
    auto interpolate_x = [](utl::Vec<int, 2> p1, utl::Vec<int, 2> p2, int y) -> int
    {
        if (p1.y() == p2.y())
            return p1.x();  // Horizontal line
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
            draw_anti_aliased_line({prev_x1, y - 1}, {x1, y}, ch);
        if (count && prev_x2 != x2)
            draw_anti_aliased_line({prev_x2, y - 1}, {x2, y}, ch);
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
            draw_anti_aliased_line({prev_x1, y - 1}, {x1, y}, ch);
        if (count && prev_x2 != x2)
            draw_anti_aliased_line({prev_x2, y - 1}, {x2, y}, ch);
        prev_x1 = x1;
        prev_x2 = x2;
        count++;
    }
}

void Renderer::draw_arc(utl::Vec<int, 2> center, int radius, char ch, float end_angle, float start_angle /* = 0.0f */,
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
                    _buffer->set({x, y}, anti_aliasing[0][0], color);
                else
                    _buffer->set({x, y}, anti_aliasing[0][1], color);
            }
            else  //Case 2: Bottom of pixel i.e. y is below the center
            {
                if (alpha <= 0.5)
                    _buffer->set({x, y}, anti_aliasing[1][0], color);
                else
                    _buffer->set({x, y}, anti_aliasing[1][1], color);
            }
        }

        angle += step;
    }
}

void Renderer::draw_text(utl::Vec<int, 2> start, const std::string &text, Color color)
{
    std::vector<Point> points;
    int x = start.x();
    int y = start.y();
    for (size_t i = 0; i < text.size() / 2; i++)
    {
        if (x >= 0 && x < static_cast<int>(_buffer->width) && y >= 0 && y < static_cast<int>(_buffer->height))
            _buffer->set({x, y}, text[i * 2], text[i * 2 + 1], color);
        x++;
    }
    if (text.size() % 2 == 1)
        _buffer->set({x, y}, text[(int)(text.length() - 1)], ' ', color);
}

void Renderer::draw_text_with_font(utl::Vec<int, 2> start, const std::string &text, Color color, const Font &font)
{
    int x = start.x();
    int y = start.y();
    int px = 0;
    int py = 0;
    int count = 0;
    for (char ch : text)
    {
        if (font.get_glyph(ch).is_empty())
            continue;
        const Font_glyph &glyph = font.get_glyph(ch);
        const std::vector<std::string> &lines = glyph.get_lines();
        // Render each line of the glyph
        // if width is odd then we need to draw last character, no biggies
        for (size_t j = 0; j < lines.size(); j++)
        {
            for (size_t k = 0; k < lines[j].size() / 2; k++)
            {
                px = x + k;
                py = y + j;
                _buffer->set({px, py}, lines[j][2 * k], lines[j][2 * k + 1], color);
            }
            if (lines.size() % 2 == 1)
            {
                px = x + lines[0].size() / 2;
                py = y + j;
                _buffer->set({px, py}, lines[j][lines[j].size() - 1], ' ', color);
            }
        }
        x += glyph.get_width() / 2 + 1;
        if (x + glyph.get_width() + 1 >= static_cast<int>(_buffer->width))
        {
            x = start.x();
            y += (glyph.get_height() + 1);
        }
    }
}

void Renderer::draw_text_with_shadow(utl::Vec<int, 2> start, const std::string &text, Color color, Color shadow_color, const Font &font,
                                     int shadow_offset_x /* = 1 */, int shadow_offset_y /* = 1 */)
{
    int x = start.x();
    int y = start.y();

    int px = 0;
    int py = 0;
    // Draw the shadow first
    for (char ch : text)
    {
        if (font.get_glyph(ch).is_empty())
            continue;
        const Font_glyph &glyph = font.get_glyph(ch);
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
        if (font.get_glyph(ch).is_empty())
            continue;
        const Font_glyph &glyph = font.get_glyph(ch);
        const std::vector<std::string> &lines = glyph.get_lines();
        // Render each line of the glyph
        for (size_t j = 0; j < lines.size(); j++)
        {
            for (size_t k = 0; k < lines[j].size(); ++k)
            {
                px = x + k;
                py = y + j * 2;
                // BUG: This uses ch1
                if ((*_buffer)(px, py)._ch1 != ' ' && lines[j][k] == ' ')
                    continue;
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
}

Font Renderer::load_font(const std::string &font_path)
{
    Font font;
    if (font.load_from_file(font_path))
        return font;
    return Font();
}

//TODO: Implement 8 bit colors too bruv
#ifdef EIGHT_BIT_COLORS

#endif  // DEBUG
void Renderer::print()
{
    std::string print_buffer;

    // Set the background color if it is not transparent
    print_buffer += _bg_color.to_ansii_bg_str();

    for (size_t y = 0; y < _buffer->height; y++)
    {
        for (size_t x = 0; x < _buffer->width; x++)
        {
            // // Check if both characters are spaces
            // if ((*_buffer)(x, y)._ch1 == ' ' && (*_buffer)(x, y)._ch2 == ' ')
            // {
            //     // Add the background color for the first space
            //     print_buffer += (*_buffer)(x, y)._color1.to_ansii_bg_str();
            //     print_buffer += ' ';
            //     // Add the background color for the second space
            //     print_buffer += (*_buffer)(x, y)._color2.to_ansii_bg_str();
            //     print_buffer += ' ';
            //     // Reset background color after each double space to the initial background color
            //     if (_bg_color != Color(TRANSPARENT))
            //         print_buffer += _bg_color.to_ansii_bg_str();
            //     continue;
            // }
            // Add the foreground color and character for _ch1
            print_buffer += (*_buffer)(x, y)._color1.to_ansii_fg_str();
            print_buffer += (*_buffer)(x, y)._ch1;
            // Add the foreground color and character for _ch2
            print_buffer += (*_buffer)(x, y)._color2.to_ansii_fg_str();
            print_buffer += (*_buffer)(x, y)._ch2;
        }
        // Add a newline at the end of each row
        print_buffer += '\n';
    }
    // Reset background color at the end of the entire buffer
    print_buffer += ANSII_BG_RESET;

    // Draw the buffer to the window
    _window.draw(print_buffer);
}

Glyph Renderer::load_glyph(const std::string &glyph_path)
{
    Glyph glyph;
    if (glyph.load_from_file(glyph_path))
        return glyph;
    return Glyph();
}

void Renderer::draw_glyph(utl::Vec<int, 2> start_pos, const Glyph &glyph, Color color /* WHITE */)
{
    auto lines = glyph.get_data();
    int px = 0;
    int py = 0;
    int x = start_pos.x();
    int y = start_pos.y();
    // if (y % 2 != 0) y--;
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
        for (size_t k = 0; k < lines[j].size(); k += 2)
        {
            px = x + k / 2;
            py = y + j;
            _buffer->set({px, py}, lines[j][k], lines[j][k + 1], color);
        }
    }
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

// TODO: Texture Mapping, animations
//       Camera system, 3D rendering,
//       key frames, procedural generation
#endif  // RENDERER_IMPLEMENTATION

#pragma once

#include <uchar.h>

#include <cmath>
#define RENDERER_IMPLEMENTATION
#include "color.hpp"
#include "renderer.hpp"
// Base class for all shapes

class Shape
{
    Color _color = WHITE;

public:
    virtual ~Shape() = default;
    virtual void draw(Renderer &renderer) = 0;
};

class Line : public Shape
{
    utl::Vec<int, 2> _start;
    utl::Vec<int, 2> _end;
    Color _color;
    char _ch = '*';

public:
    Line() : _start({0, 0}), _end({0, 0}), _ch('*') {}
    Line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char ch) : _start(start), _end(end), _ch(ch) {}
    Line(int x1, int y1, int x2, int y2, char ch, Color color = WHITE) : _start({x1, y1}), _end({x2, y2}), _ch(ch) {}
    Line(const Line &line) : _start(line._start), _end(line._end), _ch(line._ch) {}
    void draw(Renderer &renderer) override { renderer.draw_line(_start, _end, _ch); }
    void draw_anti_alias(Renderer &renderer) { renderer.draw_anti_aliased_line(_start, _end, _ch); }
    void draw_xaolin_wu(Renderer &renderer) { renderer.draw_xaolin_wu_line(_start, _end, _ch); }
    void set_start(utl::Vec<int, 2> start) { _start = start; }
    void set_end(utl::Vec<int, 2> end) { _end = end; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_start() const { return _start; }
    utl::Vec<int, 2> get_end() const { return _end; }
    char get_char() const { return _ch; }
    float get_length() const { return std::sqrt(std::pow(_end[0] - _start[0], 2) + std::pow(_end[1] - _start[1], 2)); }
};

class Circle : public Shape
{
    utl::Vec<int, 2> _center;
    int _radius;
    char _ch = '*';

public:
    Circle(utl::Vec<int, 2> center, int radius, char ch) : _center(center), _radius(radius), _ch(ch) {}
    Circle(int x, int y, int radius, char ch) : _center({x, y}), _radius(radius), _ch(ch) {}
    Circle(const Circle &circle) : _center(circle._center), _radius(circle._radius), _ch(circle._ch) {}
    void draw(Renderer &renderer) override { renderer.draw_circle(_center, _radius, _ch); }
    void fill(Renderer &renderer) { renderer.draw_fill_circle(_center, _radius, _ch); }
    void set_center(utl::Vec<int, 2> center) { _center = center; }
    void set_radius(int radius) { _radius = radius; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_center() const { return _center; }
    int get_radius() const { return _radius; }
    char get_char() const { return _ch; }
    float get_area() const { return 3.14159f * _radius * _radius; }
    float get_perimeter() const { return 2 * 3.14159f * _radius; }
    void scale(float factor) { _radius = static_cast<int>(_radius * factor); }
};

class Rectangle : public Shape
{
    utl::Vec<int, 2> _top_left;
    utl::Vec<int, 2> _bottom_right;
    int _width;
    int _height;
    char _ch = '*';

public:
    Rectangle(utl::Vec<int, 2> top_left, utl::Vec<int, 2> bottom_right, char ch) : _top_left(top_left), _bottom_right(bottom_right), _ch(ch)
    {
        _width = _bottom_right[0] - _top_left[0];
        _height = _bottom_right[1] - _top_left[1];
    }
    Rectangle(utl::Vec<int, 2> top_left, int width, int height, char ch) : _top_left(top_left), _width(width), _height(height), _ch(ch)
    {
        _bottom_right[0] = _top_left[0] + _width;
        _bottom_right[1] = _top_left[1] + _height;
    }
    Rectangle(const Rectangle &rect)
        : _top_left(rect._top_left), _bottom_right(rect._bottom_right), _width(rect._width), _height(rect._height), _ch(rect._ch)
    {
    }
    void draw(Renderer &renderer) override { renderer.draw_rectangle(_top_left, _width, _height, _ch); }
    void set_top_left(utl::Vec<int, 2> top_left) { _top_left = top_left; }
    void set_bottom_right(utl::Vec<int, 2> bottom_right) { _bottom_right = bottom_right; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_top_left() const { return _top_left; }
    utl::Vec<int, 2> get_bottom_right() const { return _bottom_right; }
    char get_char() const { return _ch; }
    float get_area() const { return (_bottom_right[0] - _top_left[0]) * (_bottom_right[1] - _top_left[1]); }
    float get_perimeter() const { return 2 * ((_bottom_right[0] - _top_left[0]) + (_bottom_right[1] - _top_left[1])); }
    void scale(float factor)
    {
        int width = (_bottom_right[0] - _top_left[0]);
        int height = (_bottom_right[1] - _top_left[1]);
        _bottom_right[0] = _top_left[0] + static_cast<int>(width * factor);
        _bottom_right[1] = _top_left[1] + static_cast<int>(height * factor);
    }
};

class Triangle : public Shape
{
    utl::Vec<int, 2> _p1;
    utl::Vec<int, 2> _p2;
    utl::Vec<int, 2> _p3;
    char _ch = '*';
    Color _color;

public:
    Triangle() : _p1({0, 0}), _p2({0, 0}), _p3({0, 0}), _ch('*'), _color(Color()) {}
    Triangle(utl::Vec<int, 2> p1, utl::Vec<int, 2> p2, utl::Vec<int, 2> p3, char ch, Color color)
        : _p1(p1), _p2(p2), _p3(p3), _ch(ch), _color(color)
    {
    }
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch) : _p1({x1, y1}), _p2({x2, y2}), _p3({x3, y3}), _ch(ch) {}
    Triangle(const Triangle &triangle) : _p1(triangle._p1), _p2(triangle._p2), _p3(triangle._p3), _ch(triangle._ch) {}
    void draw(Renderer &renderer) override { renderer.draw_triangle(_p1, _p2, _p3, _ch); }
    void draw_fill(Renderer &renderer) { renderer.draw_fill_triangle(_p1, _p2, _p3, _ch); }
    void draw_anti_alias(Renderer &renderer) { renderer.draw_antialias_triangle(_p1, _p2, _p3, _ch, _color); }
    void draw_fill_anti_alias(Renderer &renderer) { renderer.draw_fill_antialias_triangle(_p1, _p2, _p3, _ch); }
    void draw_xaolin_wu(Renderer &renderer) { renderer.draw_xaolin_wu_triangle(_p1, _p2, _p3, _ch); }
    void set_p1(utl::Vec<int, 2> p1) { _p1 = p1; }
    void set_p2(utl::Vec<int, 2> p2) { _p2 = p2; }
    void set_p3(utl::Vec<int, 2> p3) { _p3 = p3; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_p1() const { return _p1; }
    utl::Vec<int, 2> get_p2() const { return _p2; }
    utl::Vec<int, 2> get_p3() const { return _p3; }
    char get_char() const { return _ch; }
    float get_area() const { return 0.5f * std::abs((_p1[0] - _p3[0]) * (_p2[1] - _p1[1]) - (_p1[0] - _p2[0]) * (_p3[1] - _p1[1])); }
    float get_perimeter() const
    {
        return std::sqrt(std::pow(_p1[0] - _p2[0], 2) + std::pow(_p1[1] - _p2[1], 2)) +
               std::sqrt(std::pow(_p2[0] - _p3[0], 2) + std::pow(_p2[1] - _p3[1], 2)) +
               std::sqrt(std::pow(_p3[0] - _p1[0], 2) + std::pow(_p3[1] - _p1[1], 2));
    }
    Triangle rotate(float angle, utl::Vec<int, 2> center, char8_t axis = 'z')
    {
        utl::Vec<int, 2> p1 = _p1.rotate_about_center(center, angle, axis);
        utl::Vec<int, 2> p2 = _p2.rotate_about_center(center, angle, axis);
        utl::Vec<int, 2> p3 = _p3.rotate_about_center(center, angle, axis);
        return Triangle(p1, p2, p3, _ch, _color);
    }

    void scale(float factor)
    {
        utl::Vec<int, 2> center = (_p1 + _p2 + _p3) * (1 / 3);
        _p1 = center + (_p1 - center) * factor;
        _p2 = center + (_p2 - center) * factor;
        _p3 = center + (_p3 - center) * factor;
    }
};

class Polygon : public Shape
{
    std::vector<utl::Vec<int, 2>> _vertices;
    char _ch = '*';

public:
    Polygon(std::vector<utl::Vec<int, 2>> vertices, char ch) : _vertices(vertices), _ch(ch) {}
    Polygon(const Polygon &polygon) : _vertices(polygon._vertices), _ch(polygon._ch) {}
    void draw(Renderer &renderer) override { renderer.draw_polygon(_vertices, _ch); }
};

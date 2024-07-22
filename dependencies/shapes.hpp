#pragma once

#include <uchar.h>

#include <cmath>
#include <vector>

#include "color.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

// Base class for all shapes
class Shape
{
protected:
    Color _color;

public:
    Shape() : _color(WHITE) {}
    Shape(Color color) : _color(color) {}
    virtual ~Shape() = default;

    void set_color(Color color) { _color = color; }
    Color get_color() const { return _color; }
};

class half_point : public Shape
{
    utl::Vec<int, 2> _pos;
    bool _left;
    char _ch;

public:
    half_point() : Shape(), _pos({0, 0}), _left(true), _ch('*') {}
    half_point(utl::Vec<int, 2> pos, char ch, bool left, Color color) : Shape(color), _pos(pos), _left(left), _ch(ch) {}
    half_point(const half_point &point) : Shape(point._color), _pos(point._pos), _left(point._left), _ch(point._ch) {}

    void set_pos(utl::Vec<int, 2> pos) { _pos = pos; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_pos() const { return _pos; }
    char get_char() const { return _ch; }
    bool is_left() const { return _left; }
};

class Point : public Shape
{
    utl::Vec<int, 2> _pos;
    char _ch;
    char _ch2;
    Color _color2;

public:
    Point() : Shape(), _pos({0, 0}), _ch('*'), _ch2('*'), _color2(_color) {}
    Point(utl::Vec<int, 2> pos, char ch, Color color) : Shape(color), _pos(pos), _ch(ch), _ch2(ch), _color2(color) {}
    Point(utl::Vec<int, 2> pos, char ch1, char ch2, Color color) : Shape(color), _pos(pos), _ch(ch1), _ch2(ch2), _color2(color) {}
    Point(int x, int y, char ch, Color color) : Shape(color), _pos({x, y}), _ch(ch), _ch2(ch), _color2(color) {}
    Point(const Point &point) : Shape(point._color), _pos(point._pos), _ch(point._ch), _ch2(point._ch2), _color2(point._color2) {}
    Point(utl::Vec<int, 2> pos, char ch, Color color, char ch2, Color color2) : Shape(color), _pos(pos), _ch(ch), _ch2(ch2), _color2(color2)
    {
    }

    void set_pos(utl::Vec<int, 2> pos) { _pos = pos; }
    void set_char(char ch) { _ch = ch; }
    void set_char(char ch1, char ch2)
    {
        _ch = ch1;
        _ch2 = ch2;
    }
    utl::Vec<int, 2> get_pos() const { return _pos; }
    char get_char() const { return _ch; }
    char get_char2() const { return _ch2; }
    Color get_color2() const { return _color2; }
};

class Line : public Shape
{
    utl::Vec<int, 2> _start;
    utl::Vec<int, 2> _end;
    char _ch;

public:
    Line() : Shape(), _start({0, 0}), _end({0, 0}), _ch('*') {}
    Line(utl::Vec<int, 2> start, utl::Vec<int, 2> end, char ch, Color color) : Shape(color), _start(start), _end(end), _ch(ch) {}
    Line(int x1, int y1, int x2, int y2, char ch, Color color = WHITE) : Shape(color), _start({x1, y1}), _end({x2, y2}), _ch(ch) {}
    Line(const Line &line) : Shape(line._color), _start(line._start), _end(line._end), _ch(line._ch) {}

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
    char _ch;

public:
    Circle(utl::Vec<int, 2> center, int radius, char ch) : Shape(), _center(center), _radius(radius), _ch(ch) {}
    Circle(int x, int y, int radius, char ch) : Shape(), _center({x, y}), _radius(radius), _ch(ch) {}
    Circle(const Circle &circle) : Shape(circle._color), _center(circle._center), _radius(circle._radius), _ch(circle._ch) {}

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
    char _ch;

public:
    Rectangle() : Shape(), _top_left({0, 0}), _bottom_right({0, 0}), _width(0), _height(0), _ch('*') {}
    Rectangle(utl::Vec<int, 2> top_left, utl::Vec<int, 2> bottom_right, char ch, Color color)
        : Shape(color),
          _top_left(top_left),
          _bottom_right(bottom_right),
          _width(bottom_right[0] - top_left[0]),
          _height(bottom_right[1] - top_left[1]),
          _ch(ch)
    {
    }
    Rectangle(utl::Vec<int, 2> top_left, int width, int height, char ch, Color color)
        : Shape(color), _top_left(top_left), _width(width), _height(height), _ch(ch)
    {
        _bottom_right[0] = _top_left[0] + _width;
        _bottom_right[1] = _top_left[1] + _height;
    }
    Rectangle(const Rectangle &rect)
        : Shape(rect._color),
          _top_left(rect._top_left),
          _bottom_right(rect._bottom_right),
          _width(rect._width),
          _height(rect._height),
          _ch(rect._ch)
    {
    }

    void set_top_left(utl::Vec<int, 2> top_left) { _top_left = top_left; }
    void set_bottom_right(utl::Vec<int, 2> bottom_right) { _bottom_right = bottom_right; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_top_left() const { return _top_left; }
    int get_width() const { return _width; }
    int get_height() const { return _height; }
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
    utl::Vec<int, 2> _v1;
    utl::Vec<int, 2> _v2;
    utl::Vec<int, 2> _v3;
    char _ch;

public:
    Triangle() : Shape(), _v1({0, 0}), _v2({0, 0}), _v3({0, 0}), _ch('*') {}
    Triangle(utl::Vec<int, 2> v1, utl::Vec<int, 2> v2, utl::Vec<int, 2> v3, char ch, Color color)
        : Shape(color), _v1(v1), _v2(v2), _v3(v3), _ch(ch)
    {
    }
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch, Color color)
        : Shape(color), _v1({x1, y1}), _v2({x2, y2}), _v3({x3, y3}), _ch(ch)
    {
    }
    Triangle(const Triangle &triangle) : Shape(triangle._color), _v1(triangle._v1), _v2(triangle._v2), _v3(triangle._v3), _ch(triangle._ch)
    {
    }

    void set_v1(utl::Vec<int, 2> v1) { _v1 = v1; }
    void set_v2(utl::Vec<int, 2> v2) { _v2 = v2; }
    void set_v3(utl::Vec<int, 2> v3) { _v3 = v3; }
    void set_char(char ch) { _ch = ch; }
    utl::Vec<int, 2> get_v1() const { return _v1; }
    utl::Vec<int, 2> get_v2() const { return _v2; }
    utl::Vec<int, 2> get_v3() const { return _v3; }
    std::vector<utl::Vec<int, 2>> get_vertices() const { return {_v1, _v2, _v3}; }
    char get_char() const { return _ch; }
    float get_area() const
    {
        return std::abs((_v1[0] * (_v2[1] - _v3[1]) + _v2[0] * (_v3[1] - _v1[1]) + _v3[0] * (_v1[1] - _v2[1])) / 2.0f);
    }
    Triangle rotate(float angle, utl::Vec<int, 2> center, char8_t axis = 'z')
    {
        utl::Vec<int, 2> p1 = _v1.rotate_about_center(center, angle, axis);
        utl::Vec<int, 2> p2 = _v2.rotate_about_center(center, angle, axis);
        utl::Vec<int, 2> p3 = _v3.rotate_about_center(center, angle, axis);
        return Triangle(p1, p2, p3, _ch, _color);
    }
    void scale(float factor)
    {
        utl::Vec<int, 2> center = (_v1 + _v2 + _v3) * (1 / 3);
        _v1 = center + (_v1 - center) * factor;
        _v2 = center + (_v2 - center) * factor;
        _v3 = center + (_v3 - center) * factor;
    }
};

class Polygon : public Shape
{
    std::vector<utl::Vec<int, 2>> _vertices;
    char _ch;

public:
    Polygon() : Shape(), _ch('*') {}
    Polygon(std::vector<utl::Vec<int, 2>> vertices, char ch, Color color) : Shape(color), _vertices(std::move(vertices)), _ch(ch) {}
    Polygon(const Polygon &polygon) : Shape(polygon._color), _vertices(polygon._vertices), _ch(polygon._ch) {}

    void add_vertex(utl::Vec<int, 2> vertex) { _vertices.push_back(vertex); }
    void set_vertex(size_t index, utl::Vec<int, 2> vertex) { _vertices[index] = vertex; }
    void set_char(char ch) { _ch = ch; }
    std::vector<utl::Vec<int, 2>> get_vertices() const { return _vertices; }
    char get_char() const { return _ch; }

    float get_area() const
    {
        float area = 0;
        size_t n = _vertices.size();
        for (size_t i = 0; i < n; ++i)
            area += (_vertices[i][0] * _vertices[(i + 1) % n][1]) - (_vertices[(i + 1) % n][0] * _vertices[i][1]);
        return std::abs(area) / 2.0f;
    }
};

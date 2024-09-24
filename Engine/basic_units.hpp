#pragma once

#include <fstream>
#include <initializer_list>

#include "../dependencies/color.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

struct Texture_coord
{
  float u;
  float v;
  Texture_coord(std::initializer_list<float> list)
  {
    auto it = list.begin();
    if (list.size() >= 1)
      u = *it++;
    if (list.size() >= 2)
      v = *it;
  }
  Texture_coord(float u, float v) : u(u), v(v) {}
  Texture_coord() : u(0), v(0) {}
};

class Triangle3D
{
  utl::Vec<float, 3> _v1;
  utl::Vec<float, 3> _v2;
  utl::Vec<float, 3> _v3;
  Texture_coord _t1;
  Texture_coord _t2;
  Texture_coord _t3;
  char _ch;
  Color _color;

public:
  Triangle3D(utl::Vec<float, 3> v1, utl::Vec<float, 3> v2, utl::Vec<float, 3> v3, char ch, Color color)
      : _v1(v1), _v2(v2), _v3(v3), _ch(ch), _color(color)
  {
  }
  Triangle3D() : _v1({0, 0, 0}), _v2({0, 0, 0}), _v3({0, 0, 0}), _ch('.'), _color(0xffffff) {}
  Triangle3D(std::vector<utl::Vec<float, 3>> vertices) : _v1(vertices[0]), _v2(vertices[1]), _v3(vertices[2]), _ch('.'), _color(0xffffff)
  {
    if (vertices.size() != 3)
      throw std::invalid_argument("Triangle3D: Invalid number of vertices");
  }
  Triangle3D(utl::Vec<float, 3> v1, utl::Vec<float, 3> v2, utl::Vec<float, 3> v3) : _v1(v1), _v2(v2), _v3(v3) {}
  Triangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
      : _v1({x1, y1, z1}), _v2({x2, y2, z2}), _v3({x3, y3, z3}), _ch('.'), _color(0xffffff)
  {
  }

  // Copy constructor
  Triangle3D(const Triangle3D &triangle)
      : _v1(triangle._v1), _v2(triangle._v2), _v3(triangle._v3), _ch(triangle._ch), _color(triangle._color)
  {
  }

  // Copy assignment operator
  Triangle3D &operator=(const Triangle3D &triangle)
  {
    if (this != &triangle)
    {
      _v1 = triangle._v1;
      _v2 = triangle._v2;
      _v3 = triangle._v3;
      _ch = triangle._ch;
      _color = triangle._color;
    }
    return *this;
  }
  Triangle3D(std::initializer_list<utl::Vec<float, 3>> list, std::initializer_list<Texture_coord> tex_coords = {})
  {
    auto it = list.begin();
    if (list.size() >= 1)
      _v1 = *it++;
    if (list.size() >= 2)
      _v2 = *it++;
    if (list.size() >= 3)
      _v3 = *it;

    if (tex_coords.size() >= 1)
      _t1 = *tex_coords.begin();
    if (tex_coords.size() >= 2)
      _t2 = *(tex_coords.begin() + 1);
    if (tex_coords.size() >= 3)
      _t3 = *(tex_coords.begin() + 2);

    _ch = '.';
    _color = 0xffffff;
  }

  void set_v1(utl::Vec<float, 3> v1) { _v1 = v1; }
  void set_v2(utl::Vec<float, 3> v2) { _v2 = v2; }
  void set_v3(utl::Vec<float, 3> v3) { _v3 = v3; }
  void set_char(char ch) { _ch = ch; }
  void set_color(Color color) { _color = color; }
  utl::Vec<float, 3> get_v1() const { return _v1; }
  utl::Vec<float, 3> get_v2() const { return _v2; }
  utl::Vec<float, 3> get_v3() const { return _v3; }
  std::vector<utl::Vec<float, 3>> get_vertices() const { return {_v1, _v2, _v3}; }
  char get_char() const { return _ch; }
  Color get_color() const { return _color; }
  Triangle3D operator+(const utl::Vec<float, 3> &v) const { return {_v1 + v, _v2 + v, _v3 + v, _ch, _color}; }
  Triangle3D operator-(const utl::Vec<float, 3> &v) const { return {_v1 - v, _v2 - v, _v3 - v, _ch, _color}; }
  Triangle3D operator*(float scalar) const { return {_v1 * scalar, _v2 * scalar, _v3 * scalar, _ch, _color}; }
  Triangle3D operator/(float scalar) const { return {_v1 / scalar, _v2 / scalar, _v3 / scalar, _ch, _color}; }
  Triangle3D &operator+=(const utl::Vec<float, 3> &v)
  {
    _v1 += v;
    _v2 += v;
    _v3 += v;
    return *this;
  }
  Triangle3D &operator-=(const utl::Vec<float, 3> &v)
  {
    _v1 -= v;
    _v2 -= v;
    _v3 -= v;
    return *this;
  }
  Triangle3D &operator*=(float scalar)
  {
    _v1 *= scalar;
    _v2 *= scalar;
    _v3 *= scalar;
    return *this;
  }
  Triangle3D &operator/=(float scalar)
  {
    _v1 /= scalar;
    _v2 /= scalar;
    _v3 /= scalar;
    return *this;
  }
  friend Triangle3D operator*(float scalar, const Triangle3D &tri) { return tri * scalar; }
  utl::Vec<float, 3> get_normal() const
  {
    auto v1 = _v2 - _v1;
    auto v2 = _v3 - _v1;
    return v1.cross(v2).get_normalized_vector();
  }
};

struct Mesh
{
  std::vector<Triangle3D> triangles;
  Mesh() {}
  Mesh(std::vector<Triangle3D> tris) : triangles(tris) {}
  Mesh(std::initializer_list<Triangle3D> tris) : triangles(tris) {}
  void push_triangle(Triangle3D tri) { triangles.push_back(tri); }
  void push_triangles(std::vector<Triangle3D> tris) { triangles.insert(triangles.end(), tris.begin(), tris.end()); }
  void clear_triangles() { triangles.clear(); }
  static Mesh get_cube()
  {
    Mesh cube = {
        // SOUTH (front face, z = 0)
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},

        {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        // EAST (right face, x = 1)
        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},

        // NORTH (back face, z = 1)
        {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

        // WEST (left face, x = 0)
        {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

        // TOP (y = 1)
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

        // BOTTOM (y = 0)
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    };

    return cube;
  }

  bool load_from_obj(std::string file_path)
  {
    std::ifstream file(file_path);
    if (!file.is_open())
    {
      std::cerr << "Failed to open file: " << file_path << std::endl;
      return false;
    }

    std::vector<utl::Vec<float, 3>> vertices;
    std::string line;

    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      std::string prefix;
      iss >> prefix;

      if (prefix == "v")
      {
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back({x, y, z});
      }
      else if (prefix == "f")
      {
        std::string vertex1, vertex2, vertex3;
        int v1, v2, v3;

        iss >> vertex1 >> vertex2 >> vertex3;
        std::stringstream ss1(vertex1), ss2(vertex2), ss3(vertex3);

        ss1 >> v1;
        ss2 >> v2;
        ss3 >> v3;

        // OBJ indices are 1-based, convert them to 0-based
        triangles.push_back({vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1]});
      }
    }

    file.close();
    return true;
  }
};

struct Plane
{
  utl::Vec<float, 3> point;
  utl::Vec<float, 3> normal;

  Plane(utl::Vec<float, 3> point, utl::Vec<float, 3> normal) : point(point), normal(normal) {}
  Plane() : point({0, 0, 0}), normal({0, 0, 0}) {}
  Plane(float x, float y, float z, float nx, float ny, float nz) : point({x, y, z}), normal({nx, ny, nz}) {}
  Plane(std::initializer_list<utl::Vec<float, 3>> list)
  {
    auto it = list.begin();
    if (list.size() >= 1)
      point = *it++;
    if (list.size() >= 2)
      normal = *it;
  }
  void setpoint(utl::Vec<float, 3> point) { point = point; }
  void setnormal(utl::Vec<float, 3> normal) { normal = normal; }
};

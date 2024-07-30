#include <initializer_list>

#include "../dependencies/color.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

class Triangle3D
{
  utl::Vec<float, 3> _v1;
  utl::Vec<float, 3> _v2;
  utl::Vec<float, 3> _v3;

public:
  Triangle3D() : _v1({0, 0, 0}), _v2({0, 0, 0}), _v3({0, 0, 0}) {}
  Triangle3D(std::vector<utl::Vec<float, 3>> vertices) : _v1(vertices[0]), _v2(vertices[1]), _v3(vertices[2])
  {
    if (vertices.size() != 3)
      throw std::invalid_argument("Triangle3D: Invalid number of vertices");
  }
  Triangle3D(utl::Vec<float, 3> v1, utl::Vec<float, 3> v2, utl::Vec<float, 3> v3) : _v1(v1), _v2(v2), _v3(v3) {}
  Triangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
      : _v1({x1, y1, z1}), _v2({x2, y2, z2}), _v3({x3, y3, z3})
  {
  }
  Triangle3D(const Triangle3D &triangle) : _v1(triangle._v1), _v2(triangle._v2), _v3(triangle._v3) {}
  Triangle3D(std::initializer_list<utl::Vec<float, 3>> list)
  {
    auto it = list.begin();
    if (list.size() >= 1)
      _v1 = *it++;
    if (list.size() >= 2)
      _v2 = *it++;
    if (list.size() >= 3)
      _v3 = *it;
  }
  void set_v1(utl::Vec<float, 3> v1) { _v1 = v1; }
  void set_v2(utl::Vec<float, 3> v2) { _v2 = v2; }
  void set_v3(utl::Vec<float, 3> v3) { _v3 = v3; }
  utl::Vec<float, 3> get_v1() const { return _v1; }
  utl::Vec<float, 3> get_v2() const { return _v2; }
  utl::Vec<float, 3> get_v3() const { return _v3; }
  std::vector<utl::Vec<float, 3>> get_vertices() const { return {_v1, _v2, _v3}; }
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
        // WEST (left face, x = 0)
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        // NORTH (back face, z = 1)
        {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        // TOP (y = 1)
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        // BOTTOM (y = 0)
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    };

    return cube;
  }
};

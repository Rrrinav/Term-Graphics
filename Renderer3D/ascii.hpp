#pragma once

#include <cstddef>

#include "./basic_units.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"
#define RENDERER_IMPLEMENTATION4
#include "../renderer2D/ascii.hpp"

class Renderer3D : public Renderer
{
  Mesh _mesh;
  utl::Matrix<float> _projection_mat;

public:
  Renderer3D(float width, float height, float fov = 90.0f, float znear = 0.05f, float zfar = 100.0f)
      : Renderer((size_t)width, (size_t)height), _mesh(), _projection_mat(utl::Matrix<float>(4, 4))
  {
    float fov_rad = 1.0f / tan(fov * 0.5f / 180.0f * 3.14159f);
    float aspect_ratio = height / width;
    _projection_mat(0, 0) = aspect_ratio * fov_rad;
    _projection_mat(1, 1) = fov_rad;
    _projection_mat(2, 2) = zfar / (zfar - znear);
    _projection_mat(3, 2) = (-zfar * znear) / (zfar - znear);
    _projection_mat(2, 3) = 1.0f;
    _projection_mat(3, 3) = 0.0f;
  }

  void set_mesh(Mesh mesh) { _mesh = mesh; }

  std::vector<Triangle3D> get_mesh() { return _mesh.triangles; }
  utl::Matrix<float> get_projection_matrix() { return _projection_mat; }
  utl::Vec<float, 3> get_projection(utl::Vec<float, 3> point)
  {
    utl::Vec<float, 3> result;
    result[0] =
        point[0] * _projection_mat(0, 0) + point[1] * _projection_mat(1, 0) + point[2] * _projection_mat(2, 0) + _projection_mat(3, 0);
    result[1] =
        point[0] * _projection_mat(0, 1) + point[1] * _projection_mat(1, 1) + point[2] * _projection_mat(2, 1) + _projection_mat(3, 1);
    result[2] =
        point[0] * _projection_mat(0, 2) + point[1] * _projection_mat(1, 2) + point[2] * _projection_mat(2, 2) + _projection_mat(3, 2);
    float w =
        point[0] * _projection_mat(0, 3) + point[1] * _projection_mat(1, 3) + point[2] * _projection_mat(2, 3) + _projection_mat(3, 3);
    if (w != 0)
    {
      result[0] /= w;
      result[1] /= w;
      result[2] /= w;
    }
    return result;
  }
};

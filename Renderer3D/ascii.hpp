#pragma once

#include <cmath>
#include <cstddef>
#include <vector>

#include "./basic_units.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

class Engine3D : public Renderer
{
private:
  Mesh _mesh;
  utl::Matrix<float> _projection_mat;
  utl::Matrix<float> _view_matrix;
  utl::Vec<float, 3> _camera_pos;

public:
  Engine3D(float width, float height, float fov = 90.0f, float znear = 0.1f, float zfar = 1000.0f)
      : Renderer((size_t)width, (size_t)height), _mesh(), _projection_mat(4, 4), _view_matrix(4, 4), _camera_pos({0, 0, 0})
  {
    float fov_rad = 1.0f / tan(fov * 0.5f / 180.0f * 3.14159f);
    float aspect_ratio = height / width;
    _projection_mat(0, 0) = -aspect_ratio * fov_rad;
    _projection_mat(1, 1) = -fov_rad;
    _projection_mat(2, 2) = zfar / (zfar - znear);
    _projection_mat(3, 2) = (-zfar * znear) / (zfar - znear);
    _projection_mat(2, 3) = 1.0f;
    _projection_mat(3, 3) = 0.0f;
  }

  void set_mesh(const Mesh &mesh) { _mesh = mesh; }

  std::vector<Triangle3D> get_mesh() const { return _mesh.triangles; }
  utl::Matrix<float> get_projection_matrix() const { return _projection_mat; }

  utl::Vec<float, 3> get_projection(const utl::Vec<float, 3> &point) const
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

  utl::Vec<float, 3> get_camera_pos() const { return _camera_pos; }

  void set_camera_pos(const utl::Vec<float, 3> &pos) { _camera_pos = pos; }

  void update_view(const utl::Vec<float, 3> &look_dir, const utl::Vec<float, 3> &up)
  {
    utl::Vec<float, 3> target = _camera_pos + look_dir;
    utl::Matrix<float> camera_mat = _create_cam_matrix(_camera_pos, target, up);
    _view_matrix = _non_scale_inverse(camera_mat);
  }

  utl::Vec<float, 3> apply_view_transform(const utl::Vec<float, 3> &point) const
  {
    utl::Vec<float, 3> result;
    result[0] = point[0] * _view_matrix(0, 0) + point[1] * _view_matrix(1, 0) + point[2] * _view_matrix(2, 0) + _view_matrix(3, 0);
    result[1] = point[0] * _view_matrix(0, 1) + point[1] * _view_matrix(1, 1) + point[2] * _view_matrix(2, 1) + _view_matrix(3, 1);
    result[2] = point[0] * _view_matrix(0, 2) + point[1] * _view_matrix(1, 2) + point[2] * _view_matrix(2, 2) + _view_matrix(3, 2);
    float w = point[0] * _view_matrix(0, 3) + point[1] * _view_matrix(1, 3) + point[2] * _view_matrix(2, 3) + _view_matrix(3, 3);
    if (w != 0)
    {
      result[0] /= w;
      result[1] /= w;
      result[2] /= w;
    }
    return result;
  }

  void pan_cam(const utl::Vec<float, 3> &pan) { _camera_pos = _camera_pos + pan; }

private:
  utl::Matrix<float> _create_cam_matrix(const utl::Vec<float, 3> &pos, const utl::Vec<float, 3> &target, const utl::Vec<float, 3> &up)
  {
    utl::Vec<float, 3> new_forward = (target - pos).get_normalized_vector();
    utl::Vec<float, 3> a = new_forward * up.dot(new_forward);
    utl::Vec<float, 3> new_up = (up - a).get_normalized_vector();
    utl::Vec<float, 3> new_right = new_up.cross(new_forward);
    utl::Matrix<float> cam_matrix(4, 4);
    cam_matrix(0, 0) = new_right[0];
    cam_matrix(0, 1) = new_right[1];
    cam_matrix(0, 2) = new_right[2];
    cam_matrix(1, 0) = new_up[0];
    cam_matrix(1, 1) = new_up[1];
    cam_matrix(1, 2) = new_up[2];
    cam_matrix(2, 0) = new_forward[0];
    cam_matrix(2, 1) = new_forward[1];
    cam_matrix(2, 2) = new_forward[2];
    cam_matrix(3, 0) = pos[0];
    cam_matrix(3, 1) = pos[1];
    cam_matrix(3, 2) = pos[2];
    return cam_matrix;
  }

  utl::Matrix<float> _non_scale_inverse(const utl::Matrix<float> &mat)
  {
    utl::Matrix<float> result(4, 4);
    result(0, 0) = mat(0, 0);
    result(0, 1) = mat(1, 0);
    result(0, 2) = mat(2, 0);
    result(1, 0) = mat(0, 1);
    result(1, 1) = mat(1, 1);
    result(1, 2) = mat(2, 1);
    result(2, 0) = mat(0, 2);
    result(2, 1) = mat(1, 2);
    result(2, 2) = mat(2, 2);
    result(3, 0) = -(mat(3, 0) * mat(0, 0) + mat(3, 1) * mat(1, 0) + mat(3, 2) * mat(2, 0));
    result(3, 1) = -(mat(3, 0) * mat(0, 1) + mat(3, 1) * mat(1, 1) + mat(3, 2) * mat(2, 1));
    result(3, 2) = -(mat(3, 0) * mat(0, 2) + mat(3, 1) * mat(1, 2) + mat(3, 2) * mat(2, 2));
    result(0, 3) = 0;
    result(1, 3) = 0;
    result(2, 3) = 0;
    result(3, 3) = 1;
    return result;
  }
};

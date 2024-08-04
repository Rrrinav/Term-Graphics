// Almost copied from OneLoneCoder (javidx9). Though this is pretty classic and simple technique used by many.
#pragma once

#include <cmath>
#include <cstddef>
#include <vector>

#include "./basic_units.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

// BUG: Triangle clipping is not working properly.

class Engine3D : public Renderer
{
private:
  Mesh _mesh;
  utl::Matrix<float> _projection_mat;
  utl::Matrix<float> _view_matrix;
  utl::Vec<float, 3> _camera_pos;
  utl::Vec<float, 3> _look_dir;

public:
  Engine3D(float width, float height, float fov = 90.0f, float znear = 1.0f, float zfar = 1000.0f)
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
    _look_dir = {0, 0, 1};
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

  void update_view(const utl::Vec<float, 3> &up)
  {
    utl::Vec<float, 3> target = _camera_pos + _look_dir;
    utl::Matrix<float> camera_mat = _create_cam_matrix(_camera_pos, target, up);
    _view_matrix = _non_scale_inverse(camera_mat);
  }

  void rotate_look_dir(float angle, char axis)
  {
    _look_dir = (_look_dir.rotate(angle, axis)).get_normalized_vector();
    update_view({0, 1, 0});
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

  std::vector<Triangle3D> clip_triangle(const Triangle3D &tri, const utl::Vec<float, 3> &plane_p, utl::Vec<float, 3> &plane_n)
  {
    Triangle3D in_tri = tri;
    Triangle3D out_tri1, out_tri2;
    int nClippedTriangles = 0;
    nClippedTriangles = tri_clip_against_plane(plane_p, plane_n, in_tri, out_tri1, out_tri2);
    std::vector<Triangle3D> clipped_triangles;
    if (nClippedTriangles >= 1)
      clipped_triangles.push_back(out_tri1);
    if (nClippedTriangles >= 2)
      clipped_triangles.push_back(out_tri2);

    // std::cout << "clipped_triangles\n";
    // for (auto &clipped_tri : clipped_triangles)
    // {
    //   std::cout << clipped_tri.get_v1()[0] << " " << clipped_tri.get_v1()[1] << " " << clipped_tri.get_v1()[2] << std::endl;
    //   std::cout << clipped_tri.get_v2()[0] << " " << clipped_tri.get_v2()[1] << " " << clipped_tri.get_v2()[2] << std::endl;
    //   std::cout << clipped_tri.get_v3()[0] << " " << clipped_tri.get_v3()[1] << " " << clipped_tri.get_v3()[2] << std::endl;
    //   std::cout << clipped_tri.get_color().get_rgb_string() << std::endl;
    // }
    return clipped_triangles;
  }

  int tri_clip_against_plane(const utl::Vec<float, 3> &plane_p, const utl::Vec<float, 3> &planen, const Triangle3D &in_tri,
                             Triangle3D &out_tri1, Triangle3D &out_tri2)
  {
    // Ensure the plane normal is normalized
    auto plane_n = planen.get_normalized_vector();

    // Return signed shortest distance from point to plane
    auto dist = [&](utl::Vec<float, 3> p)
    {
      // p = p.get_normalized_vector();
      return (plane_n.x() * p.x() + plane_n.y() * p.y() + plane_n.z() * p.z() - (plane_n.dot(plane_p)));
    };

    // Create two temporary storage arrays to classify points either side of plane
    utl::Vec<float, 3> inside_points[3];
    int nInsidePointCount = 0;
    utl::Vec<float, 3> outside_points[3];
    int nOutsidePointCount = 0;

    // Get signed distance of each point in triangle to plane
    double d0 = dist(in_tri.get_v1());
    double d1 = dist(in_tri.get_v2());
    double d2 = dist(in_tri.get_v3());

    // Classify each point
    if (d0 >= 0)
      inside_points[nInsidePointCount++] = in_tri.get_v1();
    else
      outside_points[nOutsidePointCount++] = in_tri.get_v1();

    if (d1 >= 0)
      inside_points[nInsidePointCount++] = in_tri.get_v2();
    else
      outside_points[nOutsidePointCount++] = in_tri.get_v2();

    if (d2 >= 0)
      inside_points[nInsidePointCount++] = in_tri.get_v3();
    else
      outside_points[nOutsidePointCount++] = in_tri.get_v3();

    // Handle various cases
    if (nInsidePointCount == 0)
    {
      // All points are outside, no triangle returned
      return 0;
    }
    if (nInsidePointCount == 3)
    {
      // All points are inside, return the original triangle
      out_tri1 = in_tri;
      return 1;
    }
    if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
      // Triangle should be clipped, as two points lie outside
      out_tri1.set_color(RED);
      out_tri1.set_char(in_tri.get_char());
      out_tri1.set_v1(inside_points[0]);
      out_tri1.set_v2(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[0]));
      out_tri1.set_v3(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[1]));
      // std::cout << "out_tri only 1\n";
      // std::cout << out_tri1.get_v1()[0] << " " << out_tri1.get_v1()[1] << " " << out_tri1.get_v1()[2] << std::endl;
      return 1;
    }
    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
      // Triangle should be clipped, producing two new triangles
      out_tri1.set_color(BLUE);
      out_tri1.set_char(in_tri.get_char());
      out_tri1.set_v1(inside_points[0]);
      out_tri1.set_v2(inside_points[1]);
      out_tri1.set_v3(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[0]));

      out_tri2.set_color(GREEN);
      out_tri2.set_char(in_tri.get_char());
      out_tri2.set_v1(inside_points[1]);
      out_tri2.set_v2(out_tri1.get_v3());
      out_tri2.set_v3(_intersect_plane(plane_p, plane_n, inside_points[1], outside_points[0]));
      // std::cout << "Two triangles---------------------------------------------------------\n";
      // std::cout << out_tri1.get_v1()[0] << " " << out_tri1.get_v1()[1] << " " << out_tri1.get_v1()[2] << std::endl;
      // std::cout << out_tri1.get_v2()[0] << " " << out_tri1.get_v2()[1] << " " << out_tri1.get_v2()[2] << std::endl;
      // std::cout << out_tri1.get_v3()[0] << " " << out_tri1.get_v3()[1] << " " << out_tri1.get_v3()[2] << std::endl;
      // std::cout << out_tri2.get_v1()[0] << " " << out_tri2.get_v1()[1] << " " << out_tri2.get_v1()[2] << std::endl;
      // std::cout << out_tri2.get_v2()[0] << " " << out_tri2.get_v2()[1] << " " << out_tri2.get_v2()[2] << std::endl;
      // std::cout << out_tri2.get_v3()[0] << " " << out_tri2.get_v3()[1] << " " << out_tri2.get_v3()[2] << std::endl;
      // std::cout << "----------------------------------------\n";
      return 2;
    }

    // In case there's a logic flaw, return 0
    return 0;
  }

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

  utl::Vec<float, 3> _intersect_plane(const utl::Vec<float, 3> &plane_p, utl::Vec<float, 3> &plane_n, const utl::Vec<float, 3> &lineStart,
                                      const utl::Vec<float, 3> &lineEnd)
  {
    utl::Vec<float, 3> plane_n_normalized = plane_n.get_normalized_vector();  // Ensure plane normal is normalized
    float plane_d = -plane_n_normalized.dot(plane_p);
    float ad = lineStart.dot(plane_n_normalized);
    float bd = lineEnd.dot(plane_n_normalized);
    float t = (-plane_d - ad) / (bd - ad);
    auto l = lineEnd - lineStart;
    utl::Vec<float, 3> lineToIntersect = l * t;
    return lineStart + lineToIntersect;
  }
};

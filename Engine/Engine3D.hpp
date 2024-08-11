// Almost copied from OneLoneCoder (javidx9). Though this is pretty classic and simple technique used by many.
#pragma once

#include <cmath>
#include <cstddef>
#include <list>
#include <vector>

#include "./basic_units.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

/*!
  * Engine3D class is a simple 3D rendering engine that uses 3D triangles to render 3D objects.
  * This class is a subclass of Renderer class from renderer2D/ascii.hpp.
  */
class Engine3D : public Renderer
{
private:
  Mesh _mesh;                          //>> Mesh object to store the 3D triangles
  utl::Matrix<float> _projection_mat;  //>> Projection matrix for the camera
  utl::Matrix<float> _view_matrix;     //>> View matrix for the camera
  utl::Vec<float, 3> _camera_pos;      //>> Camera position
  utl::Vec<float, 3> _look_dir;        //>> Camera look direction

public:
  /*
  * Constructor for Engine3D class.
  * @param width Width of the screen
  * @param height Height of the screen
  * @param fov Field of view in degrees, default is 90 degrees
  * @param znear Near clipping plane, default is 1.0
  * @param zfar Far clipping plane, default is 1000.0
  * @return Engine3D object
  */
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

  /*!
  * Destructor for Engine3D class.
  */
  ~Engine3D() = default;

  /*
  * Set the mesh object for the Engine3D class.
  * @param mesh Mesh object to set
  */
  void set_mesh(const Mesh &mesh) { _mesh = mesh; }

  /*!
  * Get the mesh object from the Engine3D class.
  * @return Mesh object
  */
  std::vector<Triangle3D> get_mesh() const { return _mesh.triangles; }

  /*!
  * get_projection_matrix function returns the projection matrix of the camera.
  * @return Projection matrix of the camera
  */
  utl::Matrix<float> get_projection_matrix() const { return _projection_mat; }

  /*
  * get_view_matrix function returns projected vertex
  * @param point The point to project
  * @return Projected vertex
  */
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

  /*!
  * Get the look direction of the camera.
  * @return Look direction of the camera
  */
  utl::Vec<float, 3> get_look_dir() const { return _look_dir; }

  /*!
  * Project a 3D triangle to 2D.
  * @param tri Triangle to project
  * @return Projected triangle
  */
  Triangle3D project_triangle(const Triangle3D &tri) const
  {
    Triangle3D projected_tri = tri;
    projected_tri.set_v1(get_projection(tri.get_v1()));
    projected_tri.set_v2(get_projection(tri.get_v2()));
    projected_tri.set_v3(get_projection(tri.get_v3()));
    return projected_tri;
  }

  /*!
  * Get the camera position.
  * @return Camera position
  */
  utl::Vec<float, 3> get_camera_pos() const { return _camera_pos; }

  /*!
  * Set the camera position.
  * @param pos Camera position to set
  */
  void set_camera_pos(const utl::Vec<float, 3> &pos) { _camera_pos = pos; }

  /*!
  * Update the view matrix according to changes made to the camera.
  * @param up Up vector of the camera
  */
  void update_view(const utl::Vec<float, 3> &up)
  {
    utl::Vec<float, 3> target = _camera_pos + _look_dir;
    utl::Matrix<float> camera_mat = _create_cam_matrix(_camera_pos, target, up);
    _view_matrix = _non_scale_inverse(camera_mat);
  }

  /*!
  * Rotate the camera around the look direction.
  * @param angle Angle to rotate
  * @param axis Axis to rotate around
  */
  void rotate_look_dir(float angle, char axis)
  {
    _look_dir = (_look_dir.get_normalized_vector().rotate(angle, axis)).get_normalized_vector();
    update_view({0, 1, 0});
  }

  /*!
  * Apply view transformation to a point.
  * @param point Point to transform
  * @return Transformed point
  */
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

  /*!
  * Apply view transformation to a triangle.
  * @param tri Triangle to transform
  * @return Transformed triangle
  */
  Triangle3D apply_view_transform(const Triangle3D &tri) const
  {
    Triangle3D result = tri;
    result.set_v1(apply_view_transform(tri.get_v1()));
    result.set_v2(apply_view_transform(tri.get_v2()));
    result.set_v3(apply_view_transform(tri.get_v3()));
    return result;
  }

  /*!
  * Pan the camera.
  * @param pan Pan vector to be added to current camera position
  */
  void pan_cam(const utl::Vec<float, 3> &pan) { _camera_pos = _camera_pos + pan; }

  /*!
  * Clip a triangle against a plane.
  * @param tri Triangle to clip
  * @param plane_p Point on the plane
  * @param plane_n Normal of the plane
  * @return Clipped triangles
  */
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

    return clipped_triangles;
  }

  /*!
  * Clip a triangle against a plane.
  * @param tri Triangle to clip
  * @param plane Plane object to clip against
  * @return Clipped triangles
  */
  std::vector<Triangle3D> clip_triangle(const Triangle3D &tri, Plane plane) { return clip_triangle(tri, plane.point, plane.normal); }

  /*!
  * Clip a triangle against a plane
  * @param plane Plane object to clip against
  * @param in_tri Triangle to clip
  * @param out_tri1 Reference to the first output triangle
  * @param out_tri2 Reference to the second output triangle
  * @return Number of clipped triangles
  */
  int tri_clip_against_plane(Plane plane, const Triangle3D &in_tri, Triangle3D &out_tri1, Triangle3D &out_tri2)
  {
    return tri_clip_against_plane(plane.point, plane.normal, in_tri, out_tri1, out_tri2);
  }

  /*!
  * Clip a triangle against multiple planes.
  * @param triangle Triangle to clip
  * @param planes vector of planes to clip against
  * @return Clipped triangles
  */
  std::vector<Triangle3D> tri_clip_against_planes(const Triangle3D &triangle, std::vector<Plane> planes)
  {
    std::list<Triangle3D> listTriangles = {triangle};

    for (const auto &plane : planes)
    {
      int nTrisToAdd = 0;
      int nCurrentTriangles = listTriangles.size();

      while (nCurrentTriangles--)
      {
        Triangle3D test = listTriangles.front();
        listTriangles.pop_front();

        Triangle3D clipped[2];
        nTrisToAdd = tri_clip_against_plane(plane.point, plane.normal, test, clipped[0], clipped[1]);

        for (int w = 0; w < nTrisToAdd; w++) listTriangles.push_back(clipped[w]);
      }
    }

    return std::vector<Triangle3D>(listTriangles.begin(), listTriangles.end());
  }

  /*!
  * Clip a triangle against the screen.
  * @param triangle Triangle to clip
  * @return Clipped triangles
  */
  std::vector<Triangle3D> tri_clip_against_screen(const Triangle3D &triangle)
  {
    std::vector<Plane> screen_planes = {Plane({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
                                        Plane({0.0f, (float)get_height() - 1, 0.0f}, {0.0f, -1.0f, 0.0f}),
                                        Plane({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}),
                                        Plane({(float)get_width() - 1, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f})};
    return tri_clip_against_planes(triangle, screen_planes);
  }

  /*!
  * Main function for clipping a triangle against a plane.
  * @param plane_p Point on the plane
  * @param planen Normal of the plane
  * @param in_tri Triangle to clip
  * @param out_tri1 Reference to the first output triangle
  * @param out_tri2 Reference to the second output triangle
  * @return Number of clipped triangles
  */
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
      out_tri1.set_color(in_tri.get_color());
      out_tri1.set_char(in_tri.get_char());
      out_tri1.set_v1(inside_points[0]);
      out_tri1.set_v2(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[0]));
      out_tri1.set_v3(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[1]));

      return 1;
    }
    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
      // Triangle should be clipped, producing two new triangles
      out_tri1.set_color(in_tri.get_color());
      out_tri1.set_char(in_tri.get_char());
      out_tri1.set_v1(inside_points[0]);
      out_tri1.set_v2(inside_points[1]);
      out_tri1.set_v3(_intersect_plane(plane_p, plane_n, inside_points[0], outside_points[0]));

      out_tri2.set_color(in_tri.get_color());
      out_tri2.set_char(in_tri.get_char());
      out_tri2.set_v1(inside_points[1]);
      out_tri2.set_v2(out_tri1.get_v3());
      out_tri2.set_v3(_intersect_plane(plane_p, plane_n, inside_points[1], outside_points[0]));
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

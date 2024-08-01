#include "dependencies/color.hpp"
#define RENDERER_IMPLEMENTATION
#include "./Renderer3D/ascii.hpp"
#define L_GEBRA_IMPLEMENTATION
#include <algorithm>
#include <cmath>

#include "l_gebra/l_gebra.hpp"

int main()
{
  Engine3D r(150, 150);
  r.set_bg_color(GRAY_1);
  Mesh mesh;
  mesh.load_from_obj("./assets/axis.obj");
  r.set_mesh(mesh);
  float angle = 0;

  while (true)
  {
    Window::update_input_states();
    if (Window::is_pressed(KEY_w))
      r.pan_cam(utl::Vec<float, 3>{0, 5, 0} * 0.16);
    if (Window::is_pressed(KEY_s))
      r.pan_cam(utl::Vec<float, 3>{0, -5, 0} * 0.16);
    if (Window::is_pressed(KEY_a))
      r.pan_cam(utl::Vec<float, 3>{5, 0, 0} * 0.16);
    if (Window::is_pressed(KEY_d))
      r.pan_cam(utl::Vec<float, 3>{-5, 0, 0} * 0.16);

    r.empty();
    r.reset_screen();
    // Update the view matrix
    r.update_view({0, 0, 1}, {0, 1, 0});

    std::vector<Triangle3D> triangles_to_sort;
    for (auto &tri : r.get_mesh())
    {
      auto v1 = tri.get_v1();
      auto v2 = tri.get_v2();
      auto v3 = tri.get_v3();

      // Apply rotation transformations
      v1 = v1.rotate(angle, 'x').rotate(angle, 'y');
      v2 = v2.rotate(angle, 'x').rotate(angle, 'y');
      v3 = v3.rotate(angle, 'x').rotate(angle, 'y');

      // Translate the vertices
      v1[2] += 8;
      v2[2] += 8;
      v3[2] += 8;

      // Apply view transformation
      v1 = r.apply_view_transform(v1);
      v2 = r.apply_view_transform(v2);
      v3 = r.apply_view_transform(v3);

      // Calculate the normal and dot product for back-face culling
      auto edge1 = v2 - v1;
      auto edge2 = v3 - v1;
      auto normal = edge1.cross(edge2).get_normalized_vector();

      if (normal.magnitude() == 0)
        continue;

      auto dot = normal.dot(v1 - r.get_camera_pos());

      // Only draw the triangle if it is facing the camera
      if (dot < 0)
      {
        // Project the vertices
        v1 = r.get_projection(v1);
        v2 = r.get_projection(v2);
        v3 = r.get_projection(v3);

        auto light_dir = utl::Vec<float, 3>{0, 0, -1}.get_normalized_vector();
        auto intensity = normal.dot(light_dir);
        intensity = std::max(0.0, intensity);
        auto shade = char_gradient[(int)(intensity * (char_gradient.size() - 1))];
        auto color = grayscale_gradient[(int)(intensity * (grayscale_gradient.size() - 1))];

        v1 = v1 + utl::Vec<float, 3>({1, 1, 0});
        v2 = v2 + utl::Vec<float, 3>({1, 1, 0});
        v3 = v3 + utl::Vec<float, 3>({1, 1, 0});

        v1[0] = v1[0] * 0.5f * 150;
        v1[1] = v1[1] * 0.5f * 150;
        v2[0] = v2[0] * 0.5f * 150;
        v2[1] = v2[1] * 0.5f * 150;
        v3[0] = v3[0] * 0.5f * 150;
        v3[1] = v3[1] * 0.5f * 150;

        Triangle3D t(v1, v2, v3, shade, color);
        triangles_to_sort.push_back(t);
      }
    }

    std::sort(triangles_to_sort.begin(),
              triangles_to_sort.end(),
              [](const Triangle3D &a, const Triangle3D &b)
              { return (a.get_v1()[2] + a.get_v2()[2] + a.get_v3()[2]) / 3 > (b.get_v1()[2] + b.get_v2()[2] + b.get_v3()[2]) / 3; });

    for (auto &tri : triangles_to_sort)
    {
      auto v1 = tri.get_v1();
      auto v2 = tri.get_v2();
      auto v3 = tri.get_v3();
      char shade = tri.get_char();
      Color color = tri.get_color();
      r.draw_fill_triangle({(int)v1[0], (int)v1[1]}, {(int)v2[0], (int)v2[1]}, {(int)v3[0], (int)v3[1]}, shade, color);
    }

    r.print();
    r.sleep(1000 / 60);

    // Update the angle for rotation
    // angle += 0.01f;
    // if (angle > 2 * M_PI)
    //   angle = 0;
  }

  std::cin.get();
  return 0;
}

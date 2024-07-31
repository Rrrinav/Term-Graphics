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
  mesh.load_from_obj("./assets/teapot.obj");
  r.set_mesh(mesh);

  std::vector<char> shades = {' ', '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};
  std::vector<Color> colors = {GRAY_1,  GRAY_2,  GRAY_3,  GRAY_4,  GRAY_5,  GRAY_6,  GRAY_7,  GRAY_8,  GRAY_9,  GRAY_10,
                               GRAY_11, GRAY_12, GRAY_13, GRAY_14, GRAY_15, GRAY_16, GRAY_17, GRAY_18, GRAY_19, GRAY_20};

  float angle = 0;

  while (true)
  {
    r.empty();
    r.reset_screen();
    std::vector<Triangle3D> triangles_to_sort;

    for (auto &tri : r.get_mesh())
    {
      auto v1 = tri.get_v1();
      auto v2 = tri.get_v2();
      auto v3 = tri.get_v3();

      // Apply rotation transformations
      v1 = v1.rotate(angle, 'y');
      v2 = v2.rotate(angle, 'y');
      v3 = v3.rotate(angle, 'y');
      v1 = v1.rotate(angle, 'x');
      v2 = v2.rotate(angle, 'x');
      v3 = v3.rotate(angle, 'x');

      // Translate the vertices
      v1[2] += 7;
      v2[2] += 7;
      v3[2] += 7;

      // Calculate the normal and dot product for back-face culling
      auto edge1 = v2 - v1;
      auto edge2 = v3 - v1;
      auto normal = edge1.cross(edge2).get_normalized_vector();
      if (normal.magnitude() == 0)
        continue;
      normal = normal.get_normalized_vector();
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

        auto shade = shades[(int)(intensity * (shades.size() - 1))];
        auto color = colors[(int)(intensity * (colors.size() - 1))];

        // Normalize the vertices for screen space
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
              [](Triangle3D &a, Triangle3D &b)
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

    // Update the angle for rotation
    angle += 0.1f;
    if (angle > 2 * M_PI)
      angle = 0;
    r.print();
  }

  std::cin.get();
  return 0;
}

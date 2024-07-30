#define RENDERER_IMPLEMENTATION
#include "./Renderer3D/ascii.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "l_gebra/l_gebra.hpp"

int main()
{
  Renderer3D r(120, 100);
  r.set_bg_color(GRAY_1);
  r.set_mesh(Mesh::get_cube());

  float angle = 0;
  while (1)
  {
    r.empty();
    r.reset_screen();

    for (auto &tri : r.get_mesh())
    {
      auto v1 = tri.get_v1();
      auto v2 = tri.get_v2();
      auto v3 = tri.get_v3();

      v1 = v1.rotate(angle, 'z');
      v2 = v2.rotate(angle, 'z');
      v3 = v3.rotate(angle, 'z');
      v1 = v1.rotate(angle, 'x');
      v2 = v2.rotate(angle, 'x');
      v3 = v3.rotate(angle, 'x');

      v1[2] += 3;
      v2[2] += 3;
      v3[2] += 3;

      auto normal = (v2 - v1).cross(v3 - v1);
      normal = normal.get_normalized_vector();

      if (normal[2] < 0)
      {
        v1 = r.get_projection(v1);
        v2 = r.get_projection(v2);
        v3 = r.get_projection(v3);

        v1 = v1 + utl::Vec<float, 3>({1, 1, 0});
        v2 = v2 + utl::Vec<float, 3>({1, 1, 0});
        v3 = v3 + utl::Vec<float, 3>({1, 1, 0});
        v1[0] = v1[0] * 0.5f * 120;
        v1[1] = v1[1] * 0.5f * 100;
        v2[0] = v2[0] * 0.5f * 120;
        v2[1] = v2[1] * 0.5f * 100;
        v3[0] = v3[0] * 0.5f * 120;
        v3[1] = v3[1] * 0.5f * 100;

        r.draw_triangle({(int)v1[0], (int)v1[1]}, {(int)v2[0], (int)v2[1]}, {(int)v3[0], (int)v3[1]}, '*', WHITE);
      }
    }
    angle += 0.05f;

    r.print();
    r.sleep(1000 / 60);
  }
  return 0;
}

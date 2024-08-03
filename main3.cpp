#include "dependencies/color.hpp"
#define RENDERER_IMPLEMENTATION
#include "./Renderer3D/ascii.hpp"
#define L_GEBRA_IMPLEMENTATION
#include <algorithm>
#include <cmath>
#include <list>

#include "l_gebra/l_gebra.hpp"

int main()
{
  Engine3D r(150, 150);
  r.set_bg_color(GRAY_1);
  Mesh mesh;
  mesh.load_from_obj("./assets/axis.obj");
  r.set_mesh(mesh);
  float angle = 0.1f;
  utl::Vec<float, 3> look_dir = {0, 0, 1};
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
    if (Window::is_pressed(KEY_UP))
      r.pan_cam(utl::Vec<float, 3>{0, 0, 2} * 0.16);
    if (Window::is_pressed(KEY_DOWN))
      r.pan_cam(utl::Vec<float, 3>{0, 0, -5} * 0.16);
    if (Window::is_pressed(KEY_u))
      r.rotate_look_dir(angle, 'y');
    if (Window::is_pressed(KEY_o))
      r.rotate_look_dir(-angle, 'y');

    r.empty();
    r.reset_screen();
    // Update the view matrix
    r.update_view({0, 1, 0});

    std::vector<Triangle3D> triangles_to_sort;
    for (auto &tri : r.get_mesh())
    {
      auto v1 = tri.get_v1();
      auto v2 = tri.get_v2();
      auto v3 = tri.get_v3();

      // Apply rotation transformations
      // v1 = v1.rotate(angle, 'y');
      // v2 = v2.rotate(angle, 'y');
      // v3 = v3.rotate(angle, 'y');

      // Translate the vertices
      v1[2] += 8;
      v2[2] += 8;
      v3[2] += 8;

      // Apply view transformation
      v1 = r.apply_view_transform(v1);
      v2 = r.apply_view_transform(v2);
      v3 = r.apply_view_transform(v3);

      utl::Vec<float, 3> plane_n = {0, 0, 1};
      auto t = r.clip_triangle(Triangle3D(v1, v2, v3), {0, 0, 0.1}, plane_n);
      for (auto &tr : t)
      {
        v1 = tr.get_v1();
        v2 = tr.get_v2();
        v3 = tr.get_v3();
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

          auto light_dir = utl::Vec<float, 3>{0, 1, -1}.get_normalized_vector();
          auto intensity = normal.dot(light_dir);
          intensity = std::max(0.0, intensity);
          auto shade = char_gradient[3 + (int)(intensity * (char_gradient.size() - 3))];
          auto color = grayscale_gradient[3 + (int)(intensity * (grayscale_gradient.size() - 3))];

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
    }

    std::sort(triangles_to_sort.begin(),
              triangles_to_sort.end(),
              [](const Triangle3D &a, const Triangle3D &b)
              { return (a.get_v1()[2] + a.get_v2()[2] + a.get_v3()[2]) / 3 > (b.get_v1()[2] + b.get_v2()[2] + b.get_v3()[2]) / 3; });

    // BUG: Some triangles are lost in the clipping
    // for (auto &triToRaster : triangles_to_sort)
    // {
    //   // Clip triangles against all four screen edges, this could yield
    //   // a bunch of triangles, so create a queue that we traverse to
    //   //  ensure we only test new triangles generated against planes
    //   Triangle3D clipped[2];
    //   std::list<Triangle3D> listTriangles;
    //   int ScreenWidth = r.get_width();
    //   int ScreenHeight = r.get_height();
    //
    //   // Add initial triangle
    //   listTriangles.push_back(triToRaster);
    //   int nNewTriangles = 1;
    //
    //   for (int p = 0; p < 4; p++)
    //   {
    //     int nTrisToAdd = 0;
    //     while (nNewTriangles > 0)
    //     {
    //       // Take triangle from front of queue
    //       Triangle3D test = listTriangles.front();
    //       listTriangles.pop_front();
    //       nNewTriangles--;
    //
    //       // Clip it against a plane. We only need to test each
    //       // subsequent plane, against subsequent new triangles
    //       // as all triangles after a plane clip are guaranteed
    //       // to lie on the inside of the plane. I like how this
    //       // comment is almost completely and utterly justified
    //       switch (p)
    //       {
    //         case 0:
    //           nTrisToAdd = r.tri_clip_against_plane({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, test, clipped[0], clipped[1]);
    //           break;
    //         case 1:
    //           nTrisToAdd = r.tri_clip_against_plane({0.0f, (float)ScreenHeight, 0.0f}, {0.0f, -1.0f, 0.0f}, test, clipped[0], clipped[1]);
    //           break;
    //         case 2:
    //           nTrisToAdd = r.tri_clip_against_plane({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, test, clipped[0], clipped[1]);
    //           break;
    //         case 3:
    //           nTrisToAdd =
    //               r.tri_clip_against_plane({(float)ScreenWidth - 1, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, test, clipped[0], clipped[1]);
    //           break;
    //       }
    //
    //       // Clipping may yield a variable number of triangles, so
    //       // add these new ones to the back of the queue for subsequent
    //       // clipping against next planes
    //       for (int w = 0; w < nTrisToAdd; w++) listTriangles.push_back(clipped[w]);
    //     }
    //     nNewTriangles = listTriangles.size();
    //   }
    //
    //   // Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
    //   for (auto &t : listTriangles)
    //   {
    //     r.draw_fill_triangle({(int)t.get_v1()[0], (int)t.get_v1()[1]},
    //                          {(int)t.get_v2()[0], (int)t.get_v2()[1]},
    //                          {(int)t.get_v3()[0], (int)t.get_v3()[1]},
    //                          t.get_char(),
    //                          t.get_color());
    //     //DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, PIXEL_SOLID, FG_BLACK);
    //   }
    // }

    for (auto tri : triangles_to_sort)
    {
      r.draw_fill_triangle({(int)tri.get_v1()[0], (int)tri.get_v1()[1]},
                           {(int)tri.get_v2()[0], (int)tri.get_v2()[1]},
                           {(int)tri.get_v3()[0], (int)tri.get_v3()[1]},
                           tri.get_char(),
                           tri.get_color());
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

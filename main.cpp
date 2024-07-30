#define RENDERER_IMPLEMENTATION

#include <cmath>
#include <vector>

#include "Camera/camera2D.hpp"
#include "renderer2D/ascii.hpp"

struct vec3
{
  float x, y, z;

  vec3 operator*(float m[][4])
  {
    vec3 result;
    result.x = x * m[0][0] + y * m[1][0] + z * m[2][0] + m[3][0];
    result.y = x * m[0][1] + y * m[1][1] + z * m[2][1] + m[3][1];
    result.z = x * m[0][2] + y * m[1][2] + z * m[2][2] + m[3][2];
    float w = x * m[0][3] + y * m[1][3] + z * m[2][3] + m[3][3];
    if (w != 0)
    {
      result.x /= w;
      result.y /= w;
      result.z /= w;
    }
    return result;
  }
};

struct Mat4
{
  float m[4][4] = {0};
  Mat4 operator*(const Mat4 &other)
  {
    Mat4 result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
    return result;
  }
  vec3 operator*(const vec3 &v)
  {
    vec3 result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
    return result;
  }
};

struct Trian
{
  vec3 p[3];
};

struct Mesh
{
  std::vector<Trian> tris;
};

Mat4 rotationX(float angle)
{
  Mat4 matrix;
  float c = cos(angle);
  float s = sin(angle);
  matrix.m[0][0] = 1;
  matrix.m[1][1] = c;
  matrix.m[1][2] = -s;
  matrix.m[2][1] = s;
  matrix.m[2][2] = c;
  matrix.m[3][3] = 1;
  return matrix;
}

Mat4 rotationY(float angle)
{
  Mat4 matrix;
  float c = cos(angle);
  float s = sin(angle);
  matrix.m[0][0] = c;
  matrix.m[0][2] = s;
  matrix.m[1][1] = 1;
  matrix.m[2][0] = -s;
  matrix.m[2][2] = c;
  matrix.m[3][3] = 1;
  return matrix;
}

Mat4 translation(float x, float y, float z)
{
  Mat4 matrix;
  matrix.m[0][0] = 1;
  matrix.m[1][1] = 1;
  matrix.m[2][2] = 1;
  matrix.m[3][3] = 1;
  matrix.m[3][0] = x;
  matrix.m[3][1] = y;
  matrix.m[3][2] = z;
  return matrix;
}
Mat4 rotationZ(float angle)
{
  Mat4 matrix;
  float c = cos(angle);
  float s = sin(angle);
  matrix.m[0][0] = c;
  matrix.m[0][1] = -s;
  matrix.m[1][0] = s;
  matrix.m[1][1] = c;
  matrix.m[2][2] = 1;
  matrix.m[3][3] = 1;
  return matrix;
}
class Renderer3D
{
public:
  Mesh mesh_cube;
  Mat4 mat_proj;
  Renderer3D(float width, float height, float fov = 90.0f, float znear = 0.05f, float zfar = 1000.0f)
  {
    float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    float aspect_ratio = height / width;
    mat_proj.m[0][0] = aspect_ratio * fov_rad;
    mat_proj.m[1][1] = fov_rad;
    mat_proj.m[2][2] = zfar / (zfar - znear);
    mat_proj.m[3][2] = (-zfar * znear) / (zfar - znear);
    mat_proj.m[2][3] = 1.0f;
    mat_proj.m[3][3] = 0.0f;
    mesh_cube.tris = {//South
                      {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
                      {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
                      //East
                      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
                      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
                      //North
                      {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
                      {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                      //West
                      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
                      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
                      //Top
                      {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
                      {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
                      //Bottom
                      {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                      {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f}};
  };
};

int main()
{
  Renderer r(140, 140);
  r.set_bg_color(GRAY_1);
  r.Init();
  Renderer3D r3d(120, 90);

  float theta = 0.0f;  // Rotation angle

  while (true)
  {             // Animation loop
    r.empty();  // Clear screen
    r.reset_screen();

    Mat4 rotX = rotationX(theta * 0.5f);
    Mat4 rotZ = rotationZ(theta * 0.5f);

    for (auto &t : r3d.mesh_cube.tris)
    {
      Trian tri;
      for (int i = 0; i < 3; i++)
      {
        vec3 p = t.p[i];
        vec3 rotated = p * rotX.m * rotZ.m;
        rotated.z += 2.0f;
        rotated = rotated * r3d.mat_proj.m;
        // Scale into view
        rotated.x += 1.0f;
        rotated.y += 1.0f;
        rotated.x *= 0.5f * r.get_width();
        rotated.y *= 0.5f * r.get_height();

        tri.p[i] = rotated;
      }

      r.draw_triangle(
          {(int)tri.p[0].x, (int)tri.p[0].y}, {(int)tri.p[1].x, (int)tri.p[1].y}, {(int)tri.p[2].x, (int)tri.p[2].y}, 'o', WHITE);
    }

    r.print();
    theta += 0.1f;  // Increment rotation angle

    r.sleep(50);  // Sleep for 100ms
  }

  return 0;
}

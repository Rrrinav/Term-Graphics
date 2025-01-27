// Totally(emphasis!) inspired by OneLoneCoder's code : https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_PerlinNoise.cpp
#include <cstddef>
#include <cstdlib>
#include <vector>

#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

enum Mode
{
  MODE_1D,
  MODE_2D,
  MODE_2D_COLOR
};

class Perlin_noise
{
  size_t _size;
  size_t _2d_width;
  size_t _2d_height;
  std::vector<float> _seed_1D;
  std::vector<float> _seed_2D;

public:
  Perlin_noise() = default;
  Perlin_noise(size_t size, size_t width2D, size_t height2D) : _size(size), _2d_height(height2D), _2d_width(width2D)
  {
    _seed_1D.resize(size);
    for (size_t i = 0; i < size; i++) _seed_1D[i] = (float)rand() / (float)RAND_MAX;
    _seed_2D.resize(width2D * height2D);
    for (size_t i = 0; i < width2D * height2D; i++) _seed_2D[i] = (float)rand() / (float)RAND_MAX;
  }

  std::vector<float> get1D_noise(size_t octaves, float bias = 0.5f)
  {
    size_t count = _size;
    if (count == 0)
      return std::vector<float>();

    if (_size >> octaves < 1)
      std::cerr << "Octaves is too high for the size of the noise\n";

    std::vector<float> result;
    result.resize(count, 0.0f);
    for (size_t i = 0; i < count; i++)
    {
      float noise = 0.0f;
      float scale = 1.0f;
      float scale_acc = 0.0f;
      for (size_t o = 0; o < octaves; o++)
      {
        size_t pitch = _size >> o;
        size_t sample_1 = (i / pitch) * pitch;
        size_t sample_2 = (sample_1 + pitch) % _size;
        float blend = (float)(i - sample_1) / (float)pitch;
        float sample = (1.0f - blend) * _seed_1D[sample_1] + blend * _seed_1D[sample_2];
        noise += sample * scale;
        scale_acc += scale;
        scale /= bias;
      }
      result[i] = noise / scale_acc;
    }
    return result;
  }

  std::vector<float> get2D_noise(size_t octaves, float bias = 0.5f)
  {
    size_t count = _2d_width * _2d_height;
    if (count == 0)
      return std::vector<float>();
    if (_2d_width >> octaves < 1 || _2d_height >> octaves < 1)
      std::cerr << "Octaves is too high for the size of the noise\n";
    std::vector<float> result;
    result.resize(count, 0.0f);
    for (size_t x = 0; x < _2d_width; x++)
      for (size_t y = 0; y < _2d_height; y++)
      {
        float noise = 0.0f;
        float scale = 1.0f;
        float scale_acc = 0.0f;
        for (size_t o = 0; o < octaves; o++)
        {
          size_t pitch_x = _2d_width >> o;
          size_t pitch_y = _2d_height >> o;
          size_t sample_x1 = (x / pitch_x) * pitch_x;
          size_t sample_x2 = (sample_x1 + pitch_x) % _2d_width;
          size_t sample_y1 = (y / pitch_y) * pitch_y;
          size_t sample_y2 = (sample_y1 + pitch_y) % _2d_height;
          float blend_x = (float)(x - sample_x1) / (float)pitch_x;
          float blend_y = (float)(y - sample_y1) / (float)pitch_y;
          float sample_1 =
              (1.0f - blend_x) * _seed_2D[sample_x1 + sample_y1 * _2d_width] + blend_x * _seed_2D[sample_x2 + sample_y1 * _2d_width];
          float sample_2 =
              (1.0f - blend_x) * _seed_2D[sample_x1 + sample_y2 * _2d_width] + blend_x * _seed_2D[sample_x2 + sample_y2 * _2d_width];
          noise += (1.0f - blend_y) * sample_1 + blend_y * sample_2;
          scale_acc += scale;
          scale /= bias;
        }
        result[y * _2d_width + x] = noise / scale_acc;
      }
    return result;
  }
  void change_seed1D()
  {
    for (size_t i = 0; i < _size; i++) _seed_1D[i] = (float)rand() / (float)RAND_MAX;
  }

  void change_seed2D()
  {
    for (size_t i = 0; i < _2d_width * _2d_height; i++) _seed_2D[i] = (float)rand() / (float)RAND_MAX;
  }
};

int main()
{
  constexpr int width = 80;
  Renderer r(width, 80);
  r.set_bg_color(utl::Color_codes::GRAY_20);
  Perlin_noise perlin(width, width, 80);
  r.Init();
  int octaves = 6;
  std::vector<float> noise;
  float bias = 1.0f;
  Mode mode = MODE_1D;
  const char *shades = "-=+0$B#@";
  size_t num_shades = strlen(shades);
  std::vector<Color> gray_scale = {utl::Color_codes::GRAY_12, utl::Color_codes::BLACK};
  std::vector<Color> color_scale = {utl::Color_codes::DARK_RED,
                                    utl::Color_codes::RED,
                                    utl::Color_codes::ORANGE,
                                    utl::Color_codes::YELLOW,
                                    utl::Color_codes::GREEN,
                                    utl::Color_codes::BLUE,
                                    utl::Color_codes::MAGENTA,
                                    utl::Color_codes::WHITE};
  while (1)
  {
    Window::update_input_states();
    r.empty();
    r.reset_screen();

    if (Window::is_pressed(Keys::KEY_1))
      mode = MODE_1D;
    if (Window::is_pressed(Keys::KEY_2))
      mode = MODE_2D;
    if (Window::is_pressed(Keys::KEY_3))
      mode = MODE_2D_COLOR;

    if (Window::is_pressed(Keys::KEY_w))
    {
      octaves++;
      if (octaves > 6)
        octaves = 1;
    }
    if (Window::is_pressed(Keys::KEY_s))
    {
      if (mode == MODE_1D)
        perlin.change_seed1D();
      else if (mode == MODE_2D || mode == MODE_2D_COLOR)
        perlin.change_seed2D();
    }
    if (Window::is_pressed(Keys::KEY_a))
      bias += 0.2f;
    if (Window::is_pressed(Keys::KEY_d))
      bias -= 0.2f;

    if (mode == MODE_1D)
    {
      noise = perlin.get1D_noise(octaves, bias);
      for (int i = 0; i < width; i++)
      {
        int y = (int)(noise[i] * 40.0f);
        r.draw_line({i, y}, {i, 40}, 'C', utl::Color_codes::DARK_OLIVE);
      }
    }
    else if (mode == MODE_2D)
    {
      noise = perlin.get2D_noise(octaves, bias);
      for (size_t x = 0; x < width; x++)
      {
        for (size_t y = 0; y < 80; y++)
        {
          float noise_value = noise[y * width + x] * num_shades;
          char shade = shades[(int)((int)noise_value % (num_shades - 1))];
          Color color = gray_scale[(int)noise_value / 2 > (num_shades / 2) ? 1 : 0];

          r.draw_point({(int)x, (int)y}, shade, color);
        }
      }
    }
    else if (mode == MODE_2D_COLOR)
    {
      const char *grad = "#@";
      noise = perlin.get2D_noise(octaves, bias);
      for (size_t x = 0; x < width; x++)
      {
        for (size_t y = 0; y < 80; y++)
        {
          float noise_value = noise[y * width + x] * 8.0f;
          char shade = grad[(int)((int)noise_value / 2 < 4 ? 0 : 1)];
          Color color = color_scale[(int)((int)noise_value % (color_scale.size() - 1))];
          r.draw_point({(int)x, (int)y}, shade, color);
        }
      }
    }
    r.print();
  }
}

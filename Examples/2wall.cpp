#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

const int HEIGHT = 40;
const int WIDTH = 40;

int main()
{
  Renderer rend(WIDTH, HEIGHT);
  rend.set_bg_color(GRAY_5);
  Sprite s("../assets/wall_sprite.txt");
  float zoom_factor = 2.0f;
  while (true)
  {
    Window::update_input_states();
    rend.empty();
    rend.reset_screen();
    for (int i = 0; i < HEIGHT; i++)
    {
      for (int j = 0; j < WIDTH; j++)
      {
        // Normalize j and i to be between 0 and 1, then scale by zoom factor
        float normalized_x = (static_cast<float>(j) / WIDTH) * zoom_factor;
        float normalized_y = (static_cast<float>(i) / HEIGHT) * zoom_factor;

        // Pass zoomed normalized coordinates to get_char and get_color
        rend.draw_point({j, i}, s.get_char(normalized_x, normalized_y), s.get_color(normalized_x, normalized_y));
      }
    }

    if (Window::is_pressed(Keys::KEY_i))
    {
      zoom_factor += 0.1f;
    }
    if (Window::is_pressed(Keys::KEY_g))
    {
      zoom_factor -= 0.1f;
    }

    rend.draw_text({0, 0}, "Press 'i', 'g', 'r', 'b' or 'c'", 0xaaccff);
    rend.print();
  }
  return 0;
}

#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

const int HEIGHT = 40;
const int WIDTH = 40;

int main()
{
  Renderer rend(WIDTH, HEIGHT);
  rend.set_bg_color(GRAY_5);
  Sprite s("../assets/wall_sprite.txt");
  int tex_print_width = 10;
  int tex_print_height = 10;
  while (true)
  {
    Window::update_input_states();
    rend.empty();
    rend.reset_screen();
    for (int i = 0; i < HEIGHT; i++)
    {
      for (int j = 0; j < WIDTH; j++)
      {
        // Pass wrapped normalized coordinates to get_char and get_color
        rend.draw_point(
            {j, i}, s.get_char_un(j, i, tex_print_width, tex_print_height), s.get_color_un(j, i, tex_print_width, tex_print_height));
      }
    }

    // Key bindings to change sprite tiling size
    if (Window::is_pressed(Keys::KEY_i))
      tex_print_width += 1;  // Increase width
    if (Window::is_pressed(Keys::KEY_k))
      tex_print_width -= 1;  // Decrease width
    if (Window::is_pressed(Keys::KEY_g))
      tex_print_height += 1;  // Increase height
    if (Window::is_pressed(Keys::KEY_h))
      tex_print_height -= 1;  // Decrease height

    rend.draw_text({0, 0}, "Press 'i', 'k' to change width, 'g', 'h' to change height", 0xaaccff);
    rend.print();
  }
  return 0;
}

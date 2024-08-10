#include <cmath>
#include <string>
#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"
#include "../time/frame_rate.hpp"

int main()
{
  auto b = Renderer::create_buffer(120, 40);
  Renderer ascii(b);
  ascii.set_bg_color(GRAY_5);
  const int target_fps = 60;
  Frame_rate fps(target_fps);
  ascii.Init();
  int amp_factor = 2;
  float time = 0;
  int x;
  int e_t = 0;
  while (1)
  {
    fps.start_frame();
    Window::update_input_states();
    ascii.empty();
    ascii.reset_screen();
    if (Window::is_pressed(Keys::KEY_ESC))
      break;
    if (Window::is_pressed(Keys::KEY_a))
      ascii.draw_text({1, 3}, "<a> pressed", WHITE);
    if (Window::is_pressed(Keys::KEY_s))
      ascii.draw_text({1, 3}, "<s> pressed", WHITE);
    ascii.draw_text({1, 1}, "<b> pressed", WHITE);
    time = fps.get_elapsed_time();
    for (x = 0; x < 120; x++)
    {
      int y = 30 - 2 * (std::exp(std::sin((float)x / 3 + 5 * fps.get_elapsed_time())) +
                        std::exp(std::sin((float)x / 4 + 0.5 * fps.get_elapsed_time())) +
                        std::exp(std::sin((float)x / 3 + 9 * fps.get_elapsed_time())));
      ascii.draw_line({x % 120, 80}, {x, y}, ':', BLUE);
      ascii.draw_point({x % 120, y - 1}, 'o', RED);
    }
    auto mouse = Window::get_mouse_event();
    ascii.draw_text({1, 1}, "FPS: " + std::to_string(fps.get_actual_fps()) + " Traget_fps: " + std::to_string(target_fps), WHITE);
    ascii.draw_text({1, 2}, "Mouse: " + std::to_string(mouse.x) + " " + std::to_string(mouse.y), WHITE);

    ascii.print();
    fps.end_frame();
  }
  ascii.end();
}

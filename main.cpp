#include <string>
#define RENDERER_IMPLEMENTATION
#include "Camera/camera2D.hpp"
#include "renderer2D/ascii.hpp"

int main()
{
  Renderer r(40, 40);
  r.set_bg_color(GRAY_1);
  r.Init();

  Camera2D camera(40, 40, {0, 0});  // Start camera at center position
  camera.set_zoom_limits(0.1f, 10.0f);

  utl::Vec<int, 2> circle_position = {0, 0};  // Start circle at center of world
  const float follow_speed = 3.0f;            // Adjust this to change how quickly the camera follows
  const float delta_time = 1.0f / 60.0f;      // Assuming a 60 FPS frame rate

  while (true)
  {
    Window::update_input_states();
    r.empty();
    r.reset_screen();

    // Handle input for moving the circle
    if (Window::is_pressed(KEY_UP))
      circle_position = circle_position + utl::Vec<int, 2>({0, -5});
    if (Window::is_pressed(KEY_DOWN))
      circle_position = circle_position + utl::Vec<int, 2>({0, 5});
    if (Window::is_pressed(KEY_LEFT))
      circle_position = circle_position + utl::Vec<int, 2>({-5, 0});
    if (Window::is_pressed(KEY_RIGHT))
      circle_position = circle_position + utl::Vec<int, 2>({5, 0});

    // Handle input for zooming
    if (Window::is_pressed(KEY_w))
      camera.zoom_by(0.1f);
    if (Window::is_pressed(KEY_s))
      camera.zoom_by(-0.1f);

    // Make the camera follow the circle with a delay

    // Draw the circle at its world position
    auto screen_position = camera.world_to_screen(circle_position);

    r.draw_fill_circle(screen_position, static_cast<int>(5 * camera.get_zoom()), 'x', CYAN);

    r.draw_text({0, 0}, "Circle: " + std::to_string(circle_position[0]) + "," + std::to_string(circle_position[1]), YELLOW);
    r.draw_text(
        {0, 2}, "Camera position: " + std::to_string(camera.get_position()[0]) + "," + std::to_string(camera.get_position()[1]), YELLOW);
    // Render the scene
    float blend = follow_speed;
    camera.follow(circle_position, delta_time, 3.0f);
    r.print();

    // Frame rate control
    r.sleep(1000 / 60);  // 60 FPS
  }
  return 0;
}

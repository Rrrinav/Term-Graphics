#include <string>
#define RENDERER_IMPLEMENTATION
#include "../Camera/camera2D.hpp"
#include "../renderer2D/ascii.hpp"
#include "../time/frame_rate.hpp"

int main()
{
  Renderer r(120, 90);
  r.set_bg_color(utl::Color_codes::GRAY_2);
  r.Init();

  Camera2D camera(120, 90, {0, 0});  // Start camera at center position
  camera.set_zoom_limits(0.1f, 10.0f);

  utl::Vec<float, 2> circle_position = {0, 0};  // Start circle at center of world
  const float circle_speed = 100.0f;            // Speed of the circle movement in units per second
  const float follow_speed = 30.0f;             // Adjust this to change how quickly the camera follows
  Frame_rate frame_rate(80);

  while (true)
  {
    frame_rate.start_frame();
    Window::update_input_states();
    r.empty();
    r.reset_screen();

    // Print a grid
    // for (int i = 0; i < 120; i += 3) r.draw_line({i, 0}, {i, 90}, '|', GRAY_19);
    // for (int i = 0; i < 90; i += 3) r.draw_line({0, i}, {120, i}, '-', GRAY_19);

    // Handle input for moving the circle
    utl::Vec<float, 2> circle_velocity = {0, 0};
    if (Window::is_pressed(KEY_UP))
      circle_velocity = circle_velocity + utl::Vec<int, 2>({0, (int)-circle_speed});
    if (Window::is_pressed(KEY_DOWN))
      circle_velocity = circle_velocity + utl::Vec<float, 2>({0, circle_speed});
    if (Window::is_pressed(KEY_LEFT))
      circle_velocity = circle_velocity + utl::Vec<int, 2>({(int)-circle_speed, 0});
    if (Window::is_pressed(KEY_RIGHT))
      circle_velocity = circle_velocity + utl::Vec<int, 2>({(int)circle_speed, 0});
    //
    float delta_time = frame_rate.get_delta_time();
    // Update the circle position
    circle_position = circle_position + (circle_velocity * delta_time);

    std::string debug = "delta_time: " + std::to_string(delta_time) + " circle_velocity: " + std::to_string(circle_velocity[0]) + "," +
                        std::to_string(circle_velocity[1]);

    r.draw_text({0, 4}, std::to_string(circle_velocity[0]) + "," + std::to_string(circle_velocity[1]), utl::Color_codes::YELLOW);

    // Handle input for zooming
    if (Window::is_pressed(KEY_w))
      camera.zoom_by(0.1f);
    if (Window::is_pressed(KEY_s))
      camera.zoom_by(-0.1f);

    // Make the camera follow the circle with a delay
    camera.smooth_follow(circle_position, delta_time, 1);

    // Draw the circle at its world position
    auto screen_position = camera.world_to_screen(circle_position);
    r.draw_fill_circle(screen_position, static_cast<int>(5 * camera.get_zoom()), 'x', utl::Color_codes::CYAN);

    // Display positions and frame rate
    r.draw_text(
        {0, 0}, "Circle: " + std::to_string(circle_position[0]) + "," + std::to_string(circle_position[1]), utl::Color_codes::YELLOW);
    r.draw_text({0, 2},
                "Camera position: " + std::to_string(camera.get_position()[0]) + "," + std::to_string(camera.get_position()[1]),
                utl::Color_codes::YELLOW);
    r.draw_text({0, 6}, "Frame rate: " + std::to_string(frame_rate.get_actual_fps()), utl::Color_codes::YELLOW);
    r.draw_text({0, 8}, " " + std::to_string(frame_rate.get_actual_fps()) + " " + std::to_string(delta_time), utl::Color_codes::YELLOW);

    auto e_time = frame_rate.get_elapsed_time();
    std::string elapsed_time = "Elapsed time: " + std::to_string(e_time);
    r.draw_text({0, 10}, elapsed_time, utl::Color_codes::YELLOW);
    static utl::Vec<float, 2> last_position = circle_position;
    float actual_velocity = ((circle_position - last_position) * (1 / delta_time)).magnitude();
    last_position = circle_position;
    r.draw_text({0, 12}, "Actual velocity: " + std::to_string(actual_velocity), utl::Color_codes::YELLOW);
    int fram_count = frame_rate.get_frame_count();
    r.draw_text({0, 14}, "Frame count: " + std::to_string(fram_count), utl::Color_codes::YELLOW);
    // // Render the scene
    r.print();
    frame_rate.end_frame();
  }
  return 0;
}

#include <queue>
#include <string>

#include "window.hpp"
// #define L_GEBRA_IMPLEMENTATION
// #include "l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include <algorithm>
#include <cmath>
#include <vector>

#include "library_fonts.hpp"
#include "renderer.hpp"
#include "shapes.hpp"

int main()
{
    auto b = Renderer::create_buffer(100, 80);
    Renderer renderer(b);
    renderer.set_bg_color(GRAY_4);
    std::vector<Point> points;
    float t = 0.0f;
    const float delta_t = 0.1f;  // Increment t more for faster animation
    const int circle_center_x = 20;
    const int circle_center_y = 20;
    const int circle_radius = 10;
    const int wave_x = 30;
    Font standard_font;
    standard_font.load_from_file("slant.txt");

    // while (true)
    // {
    //     renderer.empty();
    //     renderer.reset_screen();
    //     renderer.draw_rectangle({1, 1}, 97, 77, '-', '|', BRIGHT_WHITE);
    //
    //     std::string by = "By RINAV";
    //     renderer.draw_text({95 - (int)by.length(), 46}, by, BRIGHT_RED);
    //     renderer.draw_circle({circle_center_x, circle_center_y}, circle_radius, 'o', GREEN);
    //
    //     int x = circle_center_x + static_cast<int>(circle_radius * std::sin(t));
    //     int y = circle_center_y + static_cast<int>(circle_radius * std::cos(t));
    //
    //     // Draw the line from circle center to the point on the circle
    //     renderer.draw_line({circle_center_x, circle_center_y}, {x, y}, '.', BRIGHT_YELLOW);
    //
    //     // Add the new point to the sine wave at the start
    //     points.insert(points.begin(), Point(wave_x, y, '`', RED));
    //
    //     // Draw and update all points in the sine wave
    //     for (auto &point : points)
    //     {
    //         auto pos = point.get_pos();
    //         if (pos[0] < 100)  // Check bounds to avoid overflow
    //         {
    //             point.set_pos({pos[0] + 1, pos[1]});
    //             point.draw(renderer);
    //         }
    //     }
    //
    //     // Draw a line from the end of the radius line to the first point of the sine wave
    //     if (!points.empty())
    //     {
    //         renderer.draw_line({x, y}, {points[0].get_pos()[0], points[0].get_pos()[1]}, '-', MAGENTA);
    //     }
    //
    //     // Remove points that are outside the screen
    //     points.erase(std::remove_if(points.begin(), points.end(), [](const Point &point) { return point.get_pos()[0] >= 97; }),
    //                  points.end());
    //     renderer.draw_text_with_shadow({10, 50}, "ABCGHH", BRIGHT_PURPLE, YELLOW, standard_font, 1, 1);
    //
    //     renderer.draw();
    //     t += delta_t;  // Increased step for faster animation
    //     renderer.sleep(1000 / 60);
    // }

    // int i = 0;
    // while (i < 50)
    // {
    //     renderer.empty();
    //     renderer.reset_screen();
    //     renderer.draw_text_with_font({10, 10}, std::to_string(i), GREEN, standard_font);
    //     renderer.draw();
    //     renderer.sleep(500);
    //     i++;
    // }
    int i = 0, j = 0, k = 0;
    renderer.Init();
    Sprite sprite;
    sprite.load_from_file("flower.txt");
    utl::Vec<int, 2> pos = {10, 11};
    std::queue<Keys> keys;
    while (true)
    {
        renderer.empty();
        renderer.reset_screen();
        Window::update_key_states();

        std::string debug_info = "Key States: ";
        debug_info += "W:" + std::to_string(Window::is_pressed(Keys::KEY_w)) + " ";
        debug_info += "S:" + std::to_string(Window::is_pressed(Keys::KEY_s)) + " ";
        debug_info += "A:" + std::to_string(Window::is_pressed(Keys::KEY_a)) + " ";
        debug_info += "D:" + std::to_string(Window::is_pressed(Keys::KEY_d)) + " ";
        debug_info += "ESC:" + std::to_string(Window::is_pressed(Keys::KEY_ESC));

        if (Window::is_pressed(Keys::KEY_c)) renderer.set_bg_color(Color(i % 255, j % 255, k % 255));
        if (Window::is_pressed(Keys::KEY_w)) pos[1] -= 2;
        if (Window::is_pressed(Keys::KEY_s)) pos[1] += 2;
        if (Window::is_pressed(Keys::KEY_a)) pos[0] -= 1;
        if (Window::is_pressed(Keys::KEY_d)) pos[0] += 1;
        if (Window::is_pressed(Keys::KEY_ESC)) break;
        auto mouse_pos = Window::get_mouse_pos();
        mouse_pos[1] *= 2;
        renderer.draw_sprite(pos, sprite, GREEN);
        renderer.draw_point(mouse_pos, 'o', RED);
        renderer.draw_text({0, 0}, debug_info, WHITE);  // Assuming you have a draw_text method
        renderer.draw_text({4, 4}, std::to_string(mouse_pos[0]) + " " + std::to_string(mouse_pos[1]), RED);
        renderer.draw();
        renderer.sleep(1000 / 60);
        i += 1;
        j += 2;
        k += 5;
    }

    // renderer.Init();
    // Animated_sprite animated_sprite;
    // animated_sprite.load_from_file("animation.txt");
    // utl::Vec<int, 2> start_pos = {10, 10};
    // int frame = 0;
    // int i = 0, j = 0, k = 0;
    // char key;
    //
    // while (true)
    // {
    //     Keys key = Window::check_input();
    //     renderer.empty();
    //     renderer.reset_screen();
    //     renderer.draw_text_with_font({10, 10}, "ANIM", GREEN, standard_font);
    //     renderer.draw_sprite(start_pos, animated_sprite.get_frame(frame));
    //     renderer.draw();
    //     renderer.sleep(1000 / 10);
    //     frame = (frame + 1) % animated_sprite.get_frame_count();
    //     i += 10;
    //     j += 2;
    //     k += 5;
    // }

    return 0;
}

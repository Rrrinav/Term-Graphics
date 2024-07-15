//#incl:waude <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "dependencies/shapes.hpp"

#define RENDERER_IMPLEMENTATION
#include "assets/library_fonts.hpp"
#include "renderer2D/renderer.hpp"

#define GRAVITY 80.0f
#define RAD 4

int main()
{
    auto b = Renderer::create_buffer(120, 80);
    Renderer renderer(b);
    renderer.set_bg_color(GRAY_4);
    std::vector<Point> points;
    float t = 0.0f;
    float delta_t = 1000.0f / 60;  // Increment t more for faster animation
    const int circle_center_x = 20;
    const int circle_center_y = 20;
    const int circle_radius = 10;
    const int wave_x = 30;
    Font font1;
    font1.load_from_file("assets/slant.txt");
    Font font_2;
    font_2.load_from_file("assets/small_font.txt");
    Font font3(standard_5x5_font);

    float phase1 = 3.0f;
    float phase2 = 2.0f;
    float phase3 = 5.0f;
    int amp = 1;
    int time = 0;
    float flight_coeff = 0.5;
    utl::Vec<int, 2> v = {0, 4};
    //    renderer.draw_fill_circle({20, 20}, 10, 'o', BRIGHT_BLUE, true);
    // Now we will check fps for performance

    for (;;)
    {
        Window::update_input_states();
        renderer.empty();
        renderer.reset_screen();
        if (Window::is_pressed(Keys::KEY_a))
            amp += 1;
        if (Window::is_pressed(Keys::KEY_s))
            amp -= 1;
        for (int x = 0; x < 120; x++)
        {
            int y = 100 - (50 + static_cast<int>(2 * amp * exp(std::sin(time / 7.0f + delta_t * phase1)) +
                                                 5 * exp(std::sin(time / 5.0f + delta_t * phase2)) +
                                                 3 * exp(std::sin(time / 9.0f + delta_t * phase3))));
            renderer.draw_line({x, 80}, {x, y}, ':', BLUE);
            renderer.draw_point({x, y}, 'o', RED);
            time++;
        }
        std::string amp_str = "AMP:" + std::to_string(amp);
        renderer.draw_text_with_font({1, 1}, amp_str, BRIGHT_RED, font1);
        // renderer.draw_text({1, 60}, fps_str, BRIGHT_RED);
        renderer.draw_text({3, 8}, "NOT GETTING CALCULATED EVERY FRAME BTW", BRIGHT_PURPLE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);
        // renderer.draw_triangle({10, 10}, {20, 20}, {30, 10}, 'o', BLUE);

        renderer.print();
        // phase1 += delta_t;
        // phase2 += delta_t;
        // phase3 += delta_t;
        delta_t += 0.3;

        //std::cout << "Frame: " << frames << std::endl;
        //renderer.sleep(1000 / 500);
    }
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
    //             renderer.draw_point(point);
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
    //     renderer.print();
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
    // int i = 0, j = 0, k = 0;
    // renderer.Init();
    // Sprite sprite;
    // sprite.load_from_file("flower.txt");
    // utl::Vec<int, 2> pos = {10, 11};
    // std::vector<utl::Vec<int, 2>> poss;
    // utl::Vec<int, 2> pos2 = {60, 40};
    // while (true)
    // {
    //     renderer.empty();
    //     renderer.reset_screen();
    //     Window::update_input_states();
    //
    //     std::string debug_info = "Key States: ";
    //     debug_info += "W:" + std::to_string(Window::is_pressed(Keys::KEY_w)) + " ";
    //     debug_info += "S:" + std::to_string(Window::is_pressed(Keys::KEY_s)) + " ";
    //     debug_info += "A:" + std::to_string(Window::is_pressed(Keys::KEY_a)) + " ";
    //     debug_info += "D:" + std::to_string(Window::is_pressed(Keys::KEY_d)) + " ";
    //     debug_info += "ESC:" + std::to_string(Window::is_pressed(Keys::KEY_ESC));
    //
    //     if (Window::is_pressed(Keys::KEY_c)) renderer.set_bg_color(Color(i % 255, j % 255, k % 255));
    //     std::string color_info = "Color: ";
    //     color_info = color_info + Color(i % 255, j % 255, k % 255).get_rgb_string();
    //     if (Window::is_pressed(Keys::KEY_w)) pos[1] -= 1;
    //     if (Window::is_pressed(Keys::KEY_s)) pos[1] += 1;
    //     if (Window::is_pressed(Keys::KEY_a)) pos[0] -= 1;
    //     if (Window::is_pressed(Keys::KEY_d)) pos[0] += 1;
    //     if (Window::is_pressed(Keys::KEY_ESC)) break;
    //     auto mouse_pos = Window::get_mouse_pos();
    //     renderer.draw_sprite(pos, sprite, GREEN);
    //     poss.push_back(mouse_pos);
    //     for (auto p : poss)
    //     {
    //         if ((*b)(p[0], p[1])._ch1 != '.' && (*b)(p[0], p[1])._ch1 != ' ')
    //         {
    //             Color c = (*b)(p[0], p[1])._color.blend(LIGHT_TEAL);
    //             renderer.draw_point(p, '.', YELLOW);
    //         }
    //         else
    //         {
    //             renderer.draw_point(p, '.', BLUE);
    //         }
    //     }
    //     auto mouse_event = Window::get_mouse_event();
    //     renderer.draw_point(mouse_pos, '@', RED);
    //     std::string click;
    //     if (mouse_event.event == Event_type::LEFT_CLICK) click = "LEFT_CLICK";
    //     if (mouse_event.event == Event_type::RIGHT_CLICK) click = "RIGHT_CLICK";
    //     renderer.draw_text({0, 0}, debug_info, WHITE);  // Assuming you have a draw_text method
    //     renderer.draw_text(
    //         {4, 4}, "Mouse pos: " + std::to_string(mouse_event.x) + " " + std::to_string(mouse_event.y) + " Click: " + click, RED);
    //     renderer.draw_text({4, 6}, color_info, WHITE);
    //     renderer.draw_text({4, 8}, "Click <c> to change color to 'color_info'", RED);
    //     renderer.draw_fill_circle(pos2, 10, 'o', 0xaaaaaf);
    //     renderer.print();
    //     renderer.sleep(1000 / 120);
    //
    //     i += 1;
    //     j += 2;
    //     k += 5;
    // }

    // renderer.Init();
    //
    // while (true)
    // {
    //     renderer.empty();
    //     renderer.reset_screen();
    //     Window::update_input_states();
    //     if (Window::is_pressed(Keys::KEY_ESC)) break;
    //     auto mouse = Window::get_mouse_event();
    //     std::string debug_info = "Mouse: ";
    //     debug_info += "X:" + std::to_string(mouse.x) + " ";
    //     debug_info += "Y:" + std::to_string(mouse.y) + " ";
    //     if (mouse.event == Event_type::LEFT_CLICK) debug_info += "LEFT_CLICK ";
    //     if (mouse.event == Event_type::RIGHT_CLICK)
    //     {
    //         debug_info += "RIGHT_CLICK ";
    //         renderer.set_bg_color(Color( (mouse.x * 10 + mouse.y * 20) % 255, (mouse.x * 5 + mouse.y * 7) % 255, (mouse.x * 6 + mouse.y * 4) % 255));
    //     }
    //     if (mouse.event == Event_type::MIDDLE_CLICK) debug_info += "MIDDLE_CLICK ";
    //     if (mouse.event == Event_type::SCROLL_UP) debug_info += "SCROLL_UP ";
    //     if (mouse.event == Event_type::SCROLL_DOWN) debug_info += "SCROLL_DOWN ";
    //     renderer.draw_text({0, 0}, debug_info, WHITE);
    //
    //     renderer.draw_text_with_font({10, 10}, "HELLO", GREEN, standard_font);
    //     renderer.draw();
    //     renderer.sleep(1000 / 60);
    // }
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

    // int gravity = 80;
    // utl::Vec<float, 2> c_pos = {RAD, RAD};
    // utl::Vec<float, 2> velocity = {15, 0};
    // utl::Vec<float, 2> acceleration = {0, 18};
    // delta_t = 0.16f;
    // renderer.draw_anti_aliased_line({120, 20}, {0, 60}, '-', BRIGHT_WHITE, true);
    // while (true)
    // {
    //     renderer.empty();
    //     renderer.reset_screen();
    //     renderer.draw_fill_triangle({80, 10}, {60, 40}, {160, 60}, 'l', BRIGHT_RED, true);
    //     velocity = velocity + acceleration * delta_t;
    //     c_pos = c_pos + velocity * delta_t;
    //     if (c_pos[0] + RAD >= 120 || c_pos[0] - RAD <= 0)
    //     {
    //         c_pos = {RAD, RAD};
    //         velocity = {15, 0};
    //     }
    //     if (c_pos.y() + RAD > 80)
    //     {
    //         c_pos[1] = 80 - RAD;
    //         velocity[1] = velocity[1] * (-0.5);
    //     }
    //     renderer.draw_fill_circle(c_pos, RAD, 'o', BRIGHT_BLUE);
    //     renderer.draw_text({2, 78}, std::to_string(c_pos[0]) + " " + std::to_string(c_pos[1]), BRIGHT_RED);
    //     renderer.print();
    //     renderer.sleep(1000 / 60);
    // }
    return 0;
}

#include <string>

#include "dependencies/shapes.hpp"
#include "window/window.hpp"
#define RENDERER_IMPLEMENTATION
#include "renderer2D/renderer.hpp"
#include "time/frame_rate.hpp"

void change_color(Color &brush_color, Renderer &rend)
{
    enum Mode
    {
        numeric,
        incremental
    };
    enum key_pressed
    {
        R,
        G,
        B
    };
    int r = brush_color.r();
    int g = brush_color.g();
    int b = brush_color.b();
    std::string rs = std::to_string(r);
    std::string gs = std::to_string(g);
    std::string bs = std::to_string(b);

    key_pressed key = R;
    Mode mode = incremental;

    while (!Window::is_pressed(Keys::KEY_x))
    {
        Window::update_input_states();
        rend.empty();
        rend.reset_screen();
        rend.draw_text({1, 1}, "Sprite Engine", WHITE);
        rend.draw_text({1, 2}, "Press 'R G B' to choose component and 'x' to exit", WHITE);
        rend.draw_text({1, 3}, "Press 'w s' to change value", WHITE);
        rend.draw_text({1, 4}, "Press 'n' to enter numeric value ", WHITE);
        rend.draw_text({1, 5}, "Color: " + brush_color.get_rgb_string(), Color(r, g, b));
        if (mode == incremental)
        {
            if (Window::is_pressed(Keys::KEY_r))
                key = R;
            if (Window::is_pressed(Keys::KEY_g))
                key = G;
            if (Window::is_pressed(Keys::KEY_b))
                key = B;
            if (key == B)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    b++;
                if (Window::is_pressed(Keys::KEY_s))
                    b--;
            }
            if (key == G)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    g++;
                if (Window::is_pressed(Keys::KEY_s))
                    g--;
            }
            if (key == R)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    r++;
                if (Window::is_pressed(Keys::KEY_s))
                    r--;
            }
            brush_color = Color(r, g, b);
        }
        if (mode == numeric)
        {
            if (Window::is_pressed(Keys::KEY_r))
                key = R;
            if (Window::is_pressed(Keys::KEY_g))
                key = G;
            if (Window::is_pressed(Keys::KEY_b))
                key = B;
            if (key == B)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    b++;
                if (Window::is_pressed(Keys::KEY_s))
                    b--;
            }
            if (key == G)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    g++;
                if (Window::is_pressed(Keys::KEY_s))
                    g--;
            }
            if (key == R)
            {
                if (Window::is_pressed(Keys::KEY_w))
                    r++;
                if (Window::is_pressed(Keys::KEY_s))
                    r--;
            }
            brush_color = Color(r, g, b);
        }
        rend.print();
    }
}

int main()
{
    Renderer rend(40, 40);
    rend.set_bg_color(GRAY_5);
    std::vector<char> sprite_chars;
    std::vector<Color> sprite_colors;
    int sprite_width = 10;
    int sprite_height = 10;
    const int canvas_begin_x = 2;
    const int canvas_begin_y = 7;
    const int canvas_width = 40 - 4;
    const int canvas_height = 40 - 6 - 2;
    sprite_chars.resize(sprite_width * sprite_height, 'e');
    sprite_colors.resize(sprite_width * sprite_height, WHITE);
    std::vector<half_point> points;
    char brush_r = 'o';
    char brush_l = ' ';
    Frame_rate frame_rate(40);
    Color brush_color = WHITE;
    while (true)
    {
        frame_rate.start_frame();
        Window::update_input_states();
        Mouse_event mouse;
        rend.empty();
        rend.reset_screen();
        rend.draw_text({1, 1}, "Sprite Engine", WHITE);
        rend.draw_text({10, 1}, "Press 's' to save", WHITE);
        rend.draw_text({1, 2}, "Press 'r' to reset", WHITE);
        rend.draw_text({1, 3}, "Press '<a>, <b>, <c>, <d>' to change brush", WHITE);
        rend.draw_text({1, 4}, "Color: " + brush_color.get_rgb_string(), WHITE);
        rend.draw_text({1, 5}, "Press 'o' to change color", WHITE);
        if (Window::has_mouse_moved())
        {
            mouse = Window::get_mouse_event();
            if (mouse.event == Mouse_event_type::LEFT_CLICK)
            {
                if (mouse.x * 2 >= canvas_begin_x && mouse.x * 2 <= canvas_begin_x + sprite_width && mouse.y >= canvas_begin_y &&
                    mouse.y < canvas_begin_y + sprite_height)
                {
                    points.push_back(
                        half_point({mouse.x, mouse.y}, brush_l == ' ' ? brush_r : brush_l, brush_l == ' ' ? false : true, brush_color));
                    // for (int y = 0; y < sprite_height; y++)
                    // {
                    //     for (int x = 0; x < sprite_width / 2; x++)
                    //     {
                    //         sprite_chars[y * sprite_width + 2 * x] = (rend.get_buffer().data[(canvas_begin_y + y) * 40 + x]._ch1);
                    //         sprite_chars[y * sprite_width + 2 * x + 1] = (rend.get_buffer().data[(canvas_begin_y + y) * 40 + x]._ch2);
                    //         sprite_colors[y * sprite_width + 2 * x] = (rend.get_buffer().data[(canvas_begin_y + y) * 40 + x]._color1);
                    //         sprite_colors[y * sprite_width + 2 * x + 1] =
                    //             (rend.get_buffer().data[(canvas_begin_y + y) * 40 + x * 2]._color2);
                    //     }
                    // }
                    for (int y = 0; y < sprite_height; y++)
                    {
                        for (int x = 0; x < sprite_width / 2; x++)
                        {
                            int buffer_x = canvas_begin_x + x;
                            sprite_chars[y * sprite_width + 2 * x] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._ch1;
                            sprite_chars[y * sprite_width + 2 * x + 1] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._ch2;
                            sprite_colors[y * sprite_width + 2 * x] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._color1;
                            sprite_colors[y * sprite_width + 2 * x + 1] =
                                rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._color2;
                        }
                    }
                }
            }
        }
        if (Window::is_pressed(Keys::KEY_a))
        {
            rend.draw_text({1, 4}, "Brush: Circle", WHITE);
            brush_r = 'o';
            brush_l = ' ';
        }
        else if (Window::is_pressed(Keys::KEY_b))
        {
            brush_r = '^';
            brush_l = ' ';
        }
        else if (Window::is_pressed(Keys::KEY_c))
        {
            brush_r = ' ';
            brush_l = 'o';
        }
        else if (Window::is_pressed(Keys::KEY_d))
        {
            brush_r = ' ';
            brush_l = '^';
        }

        if (Window::is_pressed(Keys::KEY_r))
        {
            points.clear();
            sprite_chars.clear();
            sprite_colors.clear();
        }
        for (auto &point : points) rend.draw_half_point(point);
        if (Window::is_pressed(Keys::KEY_o))
            change_color(brush_color, rend);
        // if (Window::is_pressed(Keys::KEY_s))
        // {
        //     for (int i = canvas_begin_y + 1; i < canvas_begin_y + canvas_height; i++)
        //         for (int j = canvas_begin_x + 1; j < canvas_begin_x + canvas_width; j++)
        //         {
        //         }
        // }
        for (auto &point : points) rend.draw_half_point(point);
        for (int y = 0; y < sprite_height; y++)
        {
            for (int x = 0; x < sprite_width / 2; x++)
            {
                int buffer_x = canvas_begin_x + x;
                sprite_chars[y * sprite_width + 2 * x] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._ch1;
                sprite_chars[y * sprite_width + 2 * x + 1] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._ch2;
                sprite_colors[y * sprite_width + 2 * x] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._color1;
                sprite_colors[y * sprite_width + 2 * x + 1] = rend.get_buffer().data[(canvas_begin_y + y) * 40 + buffer_x]._color2;
            }
        }
        Sprite s(sprite_width, sprite_height, sprite_chars, sprite_colors);
        if (Window::is_pressed(Keys::KEY_s))
            s.save_to_file("new_shit.txt");
        rend.draw_sprite({canvas_begin_x + sprite_width + 2, canvas_begin_y + sprite_height + 2}, s);
        rend.draw_half_point({mouse.x, mouse.y}, brush_l == ' ' ? brush_r : brush_l, brush_l == ' ' ? false : true, brush_color);
        rend.draw_rectangle({canvas_begin_x - 1, canvas_begin_y - 1}, canvas_width, canvas_height, '-', '|', WHITE);
        rend.print();
        frame_rate.end_frame();
    }
    return 0;
}

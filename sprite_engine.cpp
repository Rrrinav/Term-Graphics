#define RENDERER_IMPLEMENTATION

#include "renderer2D/renderer.hpp"

int main()
{
    Renderer rend(60, 60);
    rend.set_bg_color(GRAY_5);
    std::vector<char> sprite_chars;
    std::vector<Color> sprite_colors;
    int sprite_width = 10;
    int sprite_height = 10;
    const int canvas_begin_x = 1;
    const int canvas_begin_y = 6;
    const int canvas_width = 60 - 4;
    const int canvas_height = 60 - 6 - 2;
    sprite_chars.resize(sprite_width * sprite_height, 'e');
    sprite_colors.resize(sprite_width * sprite_height, WHITE);
    while (true)
    {
        Window::update_input_states();
        Sprite sprite(sprite_width, sprite_height, sprite_chars, sprite_colors);
        rend.empty();
        rend.reset_screen();
        rend.draw_text({1, 1}, "Sprite Engine", WHITE);
        rend.draw_text({1, 2}, "Press 'q' to quit", WHITE);
        rend.draw_text({1, 3}, "Press 'r' to reset", WHITE);
        if (Window::is_pressed(Keys::KEY_r))
        {
            sprite_chars.clear();
            sprite_colors.clear();
        }
        rend.draw_sprite({canvas_begin_x, canvas_begin_y}, sprite);
        rend.draw_rectangle({canvas_begin_x, canvas_begin_y}, canvas_width, canvas_height, '-', '|', WHITE);
        rend.print();
    }
    return 0;
}

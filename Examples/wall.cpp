#define RENDERER_IMPLEMENTATION
#include "../renderer2D/renderer.hpp"

int main()
{
    Renderer rend(40, 40);
    rend.set_bg_color(GRAY_5);
    Sprite s("../assets/wall_sprite.txt");
    auto characters = s.characters();
    auto colors = s.colors();
    size_t width = s.width();
    auto color_s = colors;
    while (true)
    {
        Window::update_input_states();
        rend.empty();
        rend.reset_screen();
        for (int y = 0; y < 40; y += s.height())
            for (int x = 0; x < 40; x += s.width() / 2) rend.draw_sprite({x, y}, s);
        if (Window::is_pressed(KEY_i))
        {
            for (auto &c : colors) c = c.invert();
            s.set_colors(colors);
        }
        if (Window::is_pressed(KEY_g))
        {
            for (auto &c : colors) c = c.gray_scale();
            s.set_colors(colors);
        }
        if (Window::is_pressed(KEY_r))
        {
            for (auto &c : colors) c = c.get_random();
            s.set_colors(colors);
        }
        if (Window::is_pressed(KEY_b))
        {
            for (auto &c : colors) c = c.blend(c.get_random());
            s.set_colors(colors);
        }
        if (Window::is_pressed(KEY_c))
            s.set_colors(color_s);
        rend.draw_text({0, 0}, "Press 'i', 'g', 'r', 'b' or 'c'", 0xaaccff);
        rend.print();
    }
    return 0;
}

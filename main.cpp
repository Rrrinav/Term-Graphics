#define RENDERER_IMPLEMENTATION
#include "./renderer2D/renderer.hpp"

int main()
{
    Renderer rend(20, 20);
    rend.set_bg_color(GRAY_5);
    Sprite s("assets/wall_texture.txt");
    auto characters = s.characters();
    auto colors = s.colors();
    size_t width = s.width();
    while (true)
    {
        rend.empty();
        rend.reset_screen();
        rend.draw_sprite({0, 0}, s);
        rend.print();
    }
    return 0;
}

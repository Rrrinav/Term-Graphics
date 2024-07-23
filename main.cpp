#define RENDERER_IMPLEMENTATION
#include "renderer2D/ascii.hpp"

int main()
{
    Renderer r(40, 40);
    r.set_bg_color(GRAY_1);
    while (1)
    {
        r.empty();
        r.reset_screen();
        r.print();
    }
}

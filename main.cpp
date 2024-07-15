#include "renderer2D/renderer.hpp"

int main()
{
    auto b = Renderer::create_buffer(120, 40);
    Renderer ascii(b);
    ascii.set_bg_color(GRAY_5);
    ascii.Init();
    ascii.draw_text({1, 3}, "Hello World!", WHITE);
    ascii.print();
    ascii.end();
    return 0;
}

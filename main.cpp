#define L_GEBRA_IMPLEMENTATION
#include "l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include "renderer.hpp"
#include "shapes.hpp"

int main()
{
    utl::Vec<int, 2> center = {20, 23};
    utl::Vec<int, 2> a = {10, 30};
    utl::Vec<int, 2> b = {20, 30};
    utl::Vec<int, 2> c = {30, 10};

    Triangle triangle(a, b, c, 'x');
    auto buffer = Renderer::create_buffer(60, 60);
    Renderer renderer(buffer);
    float angle = 0.0f;
    float step = 0.05f;  // Adjust this for smoother rotation

    while (true)
    {
        renderer.empty();
        Renderer::reset_screen();
        Line line;
        Triangle triangle2;
        triangle2 = triangle.rotate(angle, center);
        line.set_start({55, 30});
        line.set_end(triangle2.get_p1());
        line.draw_anti_alias(renderer);
        triangle2.draw_anti_alias(renderer);
        renderer.draw();
        Renderer::sleep(20);
        angle += step;
        if (angle >= 2 * M_PI) angle -= 2 * M_PI;
    }
}

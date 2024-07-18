#define RENDERER_IMPLEMENTATION
#include "renderer2D/ascii.hpp"

int main()
{
    Renderer r(100, 80);
    r.set_bg_color(GRAY_4);
    r.Init();

    Gradient linear_gradient;
    linear_gradient.add_color_stop(0.0f, MAGENTA);
    linear_gradient.add_color_stop(1.0f, CYAN);

    Gradient radial_gradient;
    radial_gradient.add_color_stop(0.0f, BLUE);
    radial_gradient.add_color_stop(1.0f, MAGENTA);

    while (true)
    {
        Window::update_input_states();
        r.empty();
        r.reset_screen();

        // Fill with linear gradient
        r.draw_rect_linear_gradient({10, 10}, 60, 20, 'o', radial_gradient, false);

        // Fill with radial gradient
        r.draw_rect_radial_gradient({10, 40}, 80, 30, 'o', radial_gradient);

        r.print();
    }

    return 0;
}

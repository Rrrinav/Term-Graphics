#include "window/window.hpp"
#define RENDERER_IMPLEMENTATION
#include "renderer2D/ascii.hpp"

void cb(Renderer &r) { r.set_bg_color(RED); }

int main()
{
    Renderer r(100, 80);
    r.set_bg_color(GRAY_1);
    r.Init();

    Gradient linear_gradient;
    linear_gradient.add_color_stop(0.0f, GREEN);
    linear_gradient.add_color_stop(1.0f, CYAN);

    Gradient radial_gradient;
    radial_gradient.add_color_stop(0.0f, CYAN);
    radial_gradient.add_color_stop(1.0f, PURPLE);
    auto button1 = std::make_shared<Button>(
        utl::Vec<int, 2>{10, 10}, 10, 2, ' ', BLUE, RED, "Button1", [&r]() { r.set_bg_color(Color::get_random()); });
    UI_manager manager;
    manager.add_element(button1);
    while (true)
    {
        Window::update_input_states();
        r.empty();
        r.reset_screen();
        r.draw_button(button1);
        if (Window::has_mouse_moved())
        {
            Mouse_event mouse = Window::get_mouse_event();
            manager.handle_events();
        }
        r.print();
        r.sleep(1000 / 60);
    }

    return 0;
}

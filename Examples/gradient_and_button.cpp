
#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

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
    radial_gradient.add_color_stop(0.0f, BLUE);
    radial_gradient.add_color_stop(1.0f, RED);
    radial_gradient.add_color_stop(0.5f, GREEN);
    float angle = 0.0f;
    auto button1 = std::make_shared<Button>(utl::Vec<int, 2>{10, 10}, 10, 2, ':', BLUE, RED, "Angle++", [&]() {});
    // DOUBT: Is this a bug or a feature?

    /* will run only once per click even if long click, since this executes when button is clicked and one in loop
     * executes whenever loop reaches that if statement and button is pressed at that instance, pretty`cool!
    auto button1 = std::make_shared<Button>(utl::Vec<int, 2>{10, 10}, 10, 2, ':', BLUE, RED, "Angle++", [&]() { angle += 0.1; });
     */
    auto button2 = std::make_shared<Button>(utl::Vec<int, 2>{22, 10}, 10, 2, ':', BLUE, RED, "Angle--", [&]() {});
    UI_manager manager;
    manager.add_element(button1);
    manager.add_element(button2);
    while (true)
    {
        Window::update_input_states();
        r.empty();
        r.reset_screen();
        r.draw_button(button1);
        r.draw_button(button2);
        if (Window::has_mouse_moved())
        {
            Mouse_event mouse = Window::get_mouse_event();
            manager.handle_events();
        }
        if (button1->is_pressed())
            angle += 0.01f;  // Put this code in the callback function for single click functionality
        if (button2->is_pressed())
            angle -= 0.01f;
        r.draw_rect_rotated_gradient({10, 30}, 80, 30, 'o', radial_gradient, angle);
        r.print();
        r.sleep(1000 / 60);
    }

    return 0;
}

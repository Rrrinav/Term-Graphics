#include <cctype>


#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"

void cb(Renderer &r) { r.set_bg_color(RED); }
void handle(std::shared_ptr<Textbox> text1, float &angle)
{
    std::string str;
    for (auto &ch : text1->text())
    {
        if (std::isalpha(ch))
        {
            std::cerr << ch << " is not a number";
            return;
        }
        if (std::isdigit(ch) || ch == '.')
            str += ch;
    }
    angle = std::stof(str);
}

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
    float angle = 0.0f;
    auto button1 = std::make_shared<Button>(utl::Vec<int, 2>{10, 10}, 10, 2, ':', BLUE, RED, "Angle++", [&]() { angle += 0.1; });
    auto button2 = std::make_shared<Button>(utl::Vec<int, 2>{22, 10}, 10, 2, ':', BLUE, RED, "Angle--", [&]() { angle -= 0.1; });
    auto slider1 = std::make_shared<Slider>(utl::Vec<int, 2>{10, 14}, 15, '0', BLUE, RED, [&](float value) { angle = value * 2 * M_PI; });
    auto text1 = std::make_shared<Textbox>(utl::Vec<int, 2>{10, 18}, 20, 5, ' ', BLUE, RED, "Angle (radians)");
    auto button3 = std::make_shared<Button>(utl::Vec<int, 2>{35, 18}, 10, 2, ':', BLUE, RED, "Change", [&]() { handle(text1, angle); });
    UI_manager manager;
    manager.add_element(button1);
    manager.add_element(button2);
    manager.add_element(slider1);
    manager.add_element(button3);
    while (true)
    {
        Window::update_input_states();
        r.empty();
        r.reset_screen();
        r.draw_button(button1);
        r.draw_button(button2);
        r.draw_slider(slider1);
        r.draw_textbox(text1);
        r.draw_button(button3);
        if (Window::has_mouse_moved())
        {
            Mouse_event mouse = Window::get_mouse_event();
            manager.handle_events();
            if (mouse.event == Mouse_event_type::RIGHT_RELEASE)
                r.draw_text({10, 2}, "Mouse released");
            if (button1->is_hovered())
                button1->set_bg_color(GREEN);
            else
                button1->set_bg_color(BLUE);
            if (button2->is_hovered())
                button2->set_bg_color(GREEN);
            else
                button2->set_bg_color(BLUE);
            if (button3->is_hovered())
                button3->set_bg_color(GREEN);
            else
                button3->set_bg_color(BLUE);
            if (Window::check_input() == Keys::KEY_SPACE)
                r.draw_text({10, 2}, "Space pressed");
        }
        text1->handle_event(Window::get_mouse_event());
        r.draw_rect_rotated_gradient({10, 30}, 80, 30, 'o', radial_gradient, angle);
        r.print();
        r.sleep(1000 / 60);
    }

    return 0;
}

#include "window/keys.hpp"
#define RENDERER_IMPLEMENTATION
#include <iostream>

#include "renderer2D/ascii.hpp"

int main()
{
    Renderer r(40, 40);
    while (1)
    {
        r.empty();
        r.reset_screen();
        Window::update_input_states();
        if (Window::is_pressed(Keys::KEY_F5))
            r.draw_text({1, 1}, "F5 PRESSED");
        if (Window::is_pressed(KEY_a))
            r.draw_text({1, 1}, "a pressed");
        if (Window::is_pressed(KEY_ENTER))
            r.draw_text({1, 1}, "ENTER pressed");
        if (Window::is_pressed(KEY_E))
            r.draw_text({1, 1}, "E pressed");
        if (Window::is_pressed(KEY_Ctrl_W))
            r.draw_text({1, 1}, "Ct + w pressed");
        if (Window::is_pressed(KEY_UP))
            r.draw_text({1, 1}, "up pressed");
        auto mouse = Window::get_mouse_event();
        r.draw_text({1, 2}, "Mouse X: " + std::to_string(mouse.x) + ", Mouse Y: " + std::to_string(mouse.y));
        r.print();
        r.sleep(1000 / 2);
    }
}

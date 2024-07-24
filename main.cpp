#define RENDERER_IMPLEMENTATION
#include "Camera/camera2D.hpp"
#include "renderer2D/ascii.hpp"
#include "window/window.hpp"  // Assuming this handles input like KEY_UP

int main()
{
    Renderer r(40, 40);
    r.set_bg_color(GRAY_1);
    r.Init();
    Camera2D camera(40, 40, {20, 20});  // Start camera at center position
    while (true)
    {
        Window::update_input_states();
        r.empty();
        r.reset_screen();

        // Draw a circle at the center of the camera's view
        utl::Vec<int, 2> world_center = camera.screen_to_world({20, 20});
        r.draw_fill_circle(world_center, 5 * camera.get_zoom(), 'x', CYAN);

        // Handle input for panning
        if (Window::is_pressed(KEY_UP))
            camera.pan({0, -1});
        if (Window::is_pressed(KEY_DOWN))
            camera.pan({0, 1});
        if (Window::is_pressed(KEY_LEFT))
            camera.pan({-1, 0});
        if (Window::is_pressed(KEY_RIGHT))
            camera.pan({1, 0});
        if (Window::is_pressed(KEY_w))
            camera.zoom_by(0.1);
        if (Window::is_pressed(KEY_s))
            camera.zoom_by(-0.1);

        // Render the scene
        r.print();

        // Frame rate control
        r.sleep(1000 / 60);  // 60 FPS
    }
    return 0;
}

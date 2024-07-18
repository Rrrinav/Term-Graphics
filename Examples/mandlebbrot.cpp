#include <complex>
#include <cstdlib>

#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"
#include "../time/frame_rate.hpp"

class Mandelbrot
{
    int _width, _height;
    int _max_iterations;
    float _zoom, _offsetX, _offsetY;

public:
    Mandelbrot(int width, int height, int max_iterations, float zoom = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f)
        : _width(width), _height(height), _max_iterations(max_iterations), _zoom(zoom), _offsetX(offsetX), _offsetY(offsetY)
    {
    }

    int compute(const std::complex<float> &c) const
    {
        std::complex<float> z = 0;
        int n = 0;
        while (abs(z) <= 2.0 && n < _max_iterations)
        {
            z = z * z + c;
            n++;
        }
        return n;
    }

    void render(Renderer &r) const
    {
        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                std::complex<float> c((x - _width / 2.0f) * 4.0f / (_width * _zoom) + _offsetX,
                                      (y - _height / 2.0f) * 4.0f / (_height * _zoom) + _offsetY);
                int n = compute(c);
                float t = (float)n / _max_iterations;

                // Coloring based on iterations
                Color color;
                if (n == _max_iterations)
                    color = BLACK;
                else if (t < 0.5f)
                    color = Color::lerp(DARK_BLUE, BLUE, t * 2.0f);
                else
                    color = Color::lerp(BLUE, LIGHT_BLUE, (t - 0.5f) * 2.0f);

                r.draw_point({x, y}, '#', color);
            }
        }
    }

    void zoom_in() { _zoom *= 2.0f; }

    void zoom_out() { _zoom /= 2.0f; }

    void move(float dx, float dy)
    {
        _offsetX += dx / _zoom;
        _offsetY += dy / _zoom;
    }
};

int main()
{
    Renderer r(60, 60);
    r.set_bg_color(GRAY_18);
    Mandelbrot mandelbrot(60, 60, 2000);
    r.Init();
    Frame_rate fps(10);

    while (1)
    {
        fps.start_frame();
        Window::update_input_states();
        r.empty();
        r.reset_screen();

        if (Window::is_pressed(Keys::KEY_w))
            mandelbrot.zoom_in();
        if (Window::is_pressed(Keys::KEY_s))
            mandelbrot.zoom_out();
        if (Window::is_pressed(Keys::KEY_LEFT))
            mandelbrot.move(-2.0f, 0.0f);
        if (Window::is_pressed(Keys::KEY_RIGHT))
            mandelbrot.move(2.0f, 0.0f);
        if (Window::is_pressed(Keys::KEY_UP))
            mandelbrot.move(0.0f, -2.0f);
        if (Window::is_pressed(Keys::KEY_DOWN))
            mandelbrot.move(0.0f, 2.0f);

        mandelbrot.render(r);
        r.print();
        fps.end_frame();
    }
}

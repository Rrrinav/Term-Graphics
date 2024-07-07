#define L_GEBRA_IMPLEMENTATION
#include "l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include <cmath>
#include <vector>

#include "renderer.hpp"
#include "shapes.hpp"

int main()
{
    auto b = Renderer::create_buffer(80, 80);
    Renderer renderer(b);
    std::vector<Point> points;
    float t = 0.0f;
    while (true)
    {
        renderer.empty();
        renderer.reset_screen();

        // Draw the circle
        renderer.draw_circle({20, 20}, 10, 'o');

        // Calculate the point on the circle
        int x = 20 + static_cast<int>(10 * std::sin(t));
        int y = 20 + static_cast<int>(10 * std::cos(t));

        // Draw the line from circle center to the point on the circle
        renderer.draw_line({20, 20}, {x, y}, '.');

        // Calculate the y-coordinate for the sine wave
        int x2 = 30;
        int y2 = 20 + static_cast<int>(10 * std::sin(t));

        // Add the new point to the sine wave
        points.insert(points.begin(), Point(x2, y2, '*', RED));

        // Draw all points in the sine wave
        for (auto &point : points)
        {
            point.set_pos({(int)(point.get_pos()[0] + 0.1), point.get_pos()[1]});  // Move points to the right
            point.draw(renderer);
        }

        // Draw a line from the end of the radius line to the first point of the sine wave
        if (!points.empty())
        {
            renderer.draw_line({x, y}, {points[0].get_pos()[0], points[0].get_pos()[1]}, '-');
        }

        // Remove points that are outside the screen
        points.erase(std::remove_if(points.begin(), points.end(), [](const Point &point) { return point.get_pos()[0] >= 80; }),
                     points.end());

        renderer.draw();
        t += 0.1f;  // Increased step for faster animation
        renderer.sleep(1000 / 60);
    }
}

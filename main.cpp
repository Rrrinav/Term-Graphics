#define L_GEBRA_IMPLEMENTATION
#include "l_gebra.hpp"
#define RENDERER_IMPLEMENTATION
#include <algorithm>
#include <cmath>
#include <vector>

#include "renderer.hpp"
#include "shapes.hpp"

const std::unordered_map<char, std::vector<std::string>> standard_5x5_font = {
    {'A', {"     ", " ### ", "#   #", "#####", "#   #"}}, {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
    {'C', {" ### ", "#   #", "#    ", "#   #", " ### "}}, {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
    {'E', {"#####", "#    ", "### ", "#    ", "#####"}},  {'F', {"#####", "#    ", "### ", "#    ", "#    "}},
    {'G', {" ### ", "#    ", "#  ##", "#   #", " ### "}}, {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
    {'I', {"#####", "  #  ", "  #  ", "  #  ", "#####"}}, {'J', {"#####", "   # ", "   # ", "#  # ", " ##  "}},
    {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}}, {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
    {'M', {"#   #", "## ##", "# # #", "#   #", "#   #"}}, {'N', {"#   #", "##  #", "# # #", "#  ##", "#   #"}},
    {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}}, {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
    {'Q', {" ### ", "#   #", "# # #", "#  # ", " ## #"}}, {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
    {'S', {" ####", "#    ", " ### ", "    #", "#### "}}, {'T', {"#####", "  #  ", "  #  ", "  #  ", "  #  "}},
    {'U', {"#   #", "#    ", "#   #", "#   #", " ### "}}, {'V', {"#   #", "#   #", "#   #", " # # ", "  #  "}},
    {'W', {"#   #", "#   #", "# # #", "## ##", "#   #"}}, {'X', {"#   #", " # # ", "  #  ", " # # ", "#   #"}},
    {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}}, {'Z', {"#####", "   # ", "  #  ", " #   ", "#####"}},
    {'0', {" ### ", "#  ##", "# # #", "##  #", " ### "}}, {'1', {"  #  ", " ##  ", "  #  ", "  #  ", "#####"}},
    {'2', {" ### ", "#   #", "  ## ", " #   ", "#####"}}, {'3', {" ### ", "#   #", "  ## ", "#   #", " ### "}},
    {'4', {"#   #", "#   #", "#####", "    #", "    #"}}, {'5', {"#####", "#    ", "#### ", "    #", "#### "}},
    {'6', {" ### ", "#    ", "#### ", "#   #", " ### "}}, {'7', {"#####", "   # ", "  #  ", " #   ", "#    "}},
    {'8', {" ### ", "#   #", " ### ", "#   #", " ### "}}, {'9', {" ### ", "#   #", " ####", "    #", " ### "}},
    {'.', {"     ", "     ", "     ", "     ", "  #  "}}, {',', {"     ", "     ", "     ", "  #  ", " #   "}},
    {'!', {"  #  ", "  #  ", "  #  ", "     ", "  #  "}}, {'?', {" ### ", "#   #", "  ## ", "     ", "  #  "}},
    {' ', {"     ", "     ", "     ", "     ", "     "}}, {'-', {"     ", "     ", "#####", "     ", "     "}},
    {'+', {"     ", "  #  ", "#####", "  #  ", "     "}}, {'=', {"     ", "#####", "     ", "#####", "     "}}};

int main()
{
    auto b = Renderer::create_buffer(100, 80);
    Renderer renderer(b);
    renderer.set_bg_color(GRAY_4);
    std::vector<Point> points;
    float t = 0.0f;
    const float delta_t = 0.1f;  // Increment t more for faster animation
    const int circle_center_x = 20;
    const int circle_center_y = 20;
    const int circle_radius = 10;
    const int wave_x = 30;
    Font standard_font = Font(standard_5x5_font);

    while (true)
    {
        renderer.empty();
        renderer.reset_screen();
        renderer.draw_rectangle({1, 1}, 97, 77, '-', '|', BRIGHT_WHITE);
        std::string by = "By RINAV";
        renderer.draw_text({95 - (int)by.length(), 46}, by, BRIGHT_RED);
        // Draw the circle
        renderer.draw_circle({circle_center_x, circle_center_y}, circle_radius, 'o', GREEN);

        // Calculate the point on the circle
        int x = circle_center_x + static_cast<int>(circle_radius * std::sin(t));
        int y = circle_center_y + static_cast<int>(circle_radius * std::cos(t));

        // Draw the line from circle center to the point on the circle
        renderer.draw_line({circle_center_x, circle_center_y}, {x, y}, '.', BRIGHT_YELLOW);

        // Add the new point to the sine wave at the start
        points.insert(points.begin(), Point(wave_x, y, '`', RED));

        // Draw and update all points in the sine wave
        for (auto &point : points)
        {
            auto pos = point.get_pos();
            if (pos[0] < 100)  // Check bounds to avoid overflow
            {
                point.set_pos({pos[0] + 1, pos[1]});
                point.draw(renderer);
            }
        }

        // Draw a line from the end of the radius line to the first point of the sine wave
        if (!points.empty())
        {
            renderer.draw_line({x, y}, {points[0].get_pos()[0], points[0].get_pos()[1]}, '-', MAGENTA);
        }

        // Remove points that are outside the screen
        points.erase(std::remove_if(points.begin(), points.end(), [](const Point &point) { return point.get_pos()[0] >= 97; }),
                     points.end());

        renderer.draw();
        t += delta_t;  // Increased step for faster animation
        renderer.sleep(1000 / 60);
    }
}

#include "dependencies/sprites.hpp"

int main()
{
    Sprite s("assets/Sprite.txt");
    auto characters = s.characters();
    auto colors = s.colors();
    size_t width = s.width();

    for (size_t y = 0; y < s.height(); y++)
    {
        for (size_t x = 0; x < s.width(); x++) std::cout << COUT_FG_CODE(colors[y * width + x].to_ansii()) << characters[y * width + x];
        std::cout << '\n';  // Newline after finishing the row
    }

    Sprite t("Sprite2.txt");
    auto characters2 = t.characters();
    auto colors2 = t.colors();
    size_t width2 = t.width();
    for (size_t y = 0; y < t.height(); y++)
    {
        for (size_t x = 0; x < t.width(); x++) std::cout << COUT_FG_CODE(colors2[y * width2 + x].to_ansii()) << characters2[y * width2 + x];
        std::cout << '\n';  // Newline after finishing the row
    }
    return 0;
}

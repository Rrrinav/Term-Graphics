#define RENDERER_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include "../dependencies/image.hpp"

int main()
{
  utl::Image image("../assets/test_r.png");
  size_t width = image.get_width();
  size_t height = image.get_height();

  std::cout << "=============================================== WITHOUT COLOR << ========================================================="
            << std::endl;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x) std::cout << image.pixelToChar(x, y);
    std::cout << std::endl;
  }
  
  std::cin.get();
  // Clear the screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << std::endl;
  std::cout << "=============================================== WITH COLOR << ========================================================="
            << std::endl;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x) std::cout << image.get_color(x, y).to_ansii_fg_str() << image.pixelToChar(x, y);
    std::cout << std::endl;
  }
  std::cin.get();
  return 0;
}

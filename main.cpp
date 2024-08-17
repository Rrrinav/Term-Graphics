#define RENDERER_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include "dependencies/image.hpp"
#include "renderer2D/ascii.hpp"

int main()
{
  utl::Image image("./assets/test_r.png");
  int width = image.getWidth();
  int height = image.getHeight();
  int channels = image.getChannels();
  Renderer r(width / 2, height);
  const unsigned char *pixels = image.getPixels();
  Sprite s(width, height);
  s = image.convert_image_to_sprite();
  r.draw_sprite({0, 0}, s);
  r.print();
  std::cin.get();
  // for (int y = 0; y < height; ++y)
  // {
  //   for (int x = 0; x < width; ++x)
  //   {
  //     char c = image.pixelToChar(x, y);
  //     Color clr = image.getColor(x, y);
  //     std::cout << clr.to_ansii_fg_str()  << c;
  //   }
  //   std::cout << std::endl;
  // }
  return 0;
}

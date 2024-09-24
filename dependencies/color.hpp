#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "color_codes.hpp"

#define RESET "\u001b[0m"
#define RESET_BG "\u001b[49m"
#define BOLD "\u001b[1m"

#ifdef EIGHT_BIT_COLOR

enum class Color
{
  Black = 0,
  Red = 1,
  Green = 2,
  Yellow = 3,
  Blue = 4,
  Magenta = 5,
  Cyan = 6,
  White = 7,
  Bright_Black,
  Bright_Red,
  Bright_Green,
  Bright_Yellow,
  Bright_Blue,
  Bright_Magenta,
  Bright_Cyan,
  Bright_White
};

std::string to_ansii_fg_str(Color c)
{
  switch (c)
  {
    case Color::Black:
      return "\u001b[30m";
    case Color::Red:
      return "\u001b[31m";
    case Color::Green:
      return "\u001b[32m";
    case Color::Yellow:
      return "\u001b[33m";
    case Color::Blue:
      return "\u001b[34m";
    case Color::Magenta:
      return "\u001b[35m";
    case Color::Cyan:
      return "\u001b[36m";
    case Color::White:
      return "\u001b[37m";
    case Color::Bright_Black:
      return "\u001b[30;1m";
    case Color::Bright_Red:
      return "\u001b[31;1m";
    case Color::Bright_Green:
      return "\u001b[32;1m";
    case Color::Bright_Yellow:
      return "\u001b[33;1m";
    case Color::Bright_Blue:
      return "\u001b[34;1m";
    case Color::Bright_Magenta:
      return "\u001b[35;1m";
    case Color::Bright_Cyan:
      return "\u001b[36;1m";
    case Color::Bright_White:
      return "\u001b[37;1m";
    default:
      return "";
  }
}

std::string to_ansii_bg_str(Color c)
{
  if (c > Color::White)
    c = static_cast<Color>(static_cast<int>(c) - 8);
  switch (c)
  {
    case Color::Black:
      return "\u001b[40m";
    case Color::Red:
      return "\u001b[41m";
    case Color::Green:
      return "\u001b[42m";
    case Color::Yellow:
      return "\u001b[43m";
    case Color::Blue:
      return "\u001b[44m";
    case Color::Magenta:
      return "\u001b[45m";
    case Color::Cyan:
      return "\u001b[46m";
    case Color::White:
      return "\u001b[47m";
    default:
      return "";
  }
}

#else

#ifndef SIXTEEN_BIT_COLOR
#define SIXTEEN_BIT_COLOR

#define COUT_FG_CODE(i) "\u001b[38;5;" << i << "m"
#define COUT_BG_CODE(i) "\u001b[48;5;" << i << "m"

class Color
{
  uint8_t _r;
  uint8_t _g;
  uint8_t _b;
  uint8_t _a = 255;

public:
  // Constructors 1. Default 2. RGB 3. Hex 4. ansii terminal code
  Color() : _r(0), _g(0), _b(0) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b) {}
  Color(const uint32_t &hex_val)
  {
    if (hex_val == 0x00000000)
    {
      _r = 0;
      _g = 0;
      _b = 0;
      _a = 0;
    }
    else
    {
      _r = (hex_val >> 16) & 0xFF;
      _g = (hex_val >> 8) & 0xFF;
      _b = (hex_val >> 0) & 0xFF;
      _a = 255;
    }
  }

  Color(utl::Color_codes code) : Color(static_cast<uint32_t>(code)) {}

  Color &operator=(const uint32_t &hex_val)
  {
    _r = (hex_val >> 16) & 0xFF;
    _g = (hex_val >> 8) & 0xFF;
    _b = (hex_val >> 0) & 0xFF;
    _a = 255;
    return *this;
  }

  Color &operator=(const Color &c)
  {
    if (this != &c)
    {
      _r = c.r();
      _g = c.g();
      _b = c.b();
      _a = c.a();
    }
    return *this;
  }

  bool operator!=(const uint32_t &hex_val) const
  {
    return _r != ((hex_val >> 16) & 0xFF) || _g != ((hex_val >> 8) & 0xFF) || _b != ((hex_val >> 0) & 0xFF);
  }

  bool operator!=(const Color &c) const { return _r != c.r() || _g != c.g() || _b != c.b() || _a != c.a(); }

  bool operator==(const Color &c) const { return _r == c.r() && _g == c.g() && _b == c.b() && _a == c.a(); }

  Color(const Color &c) : _r(c.r()), _g(c.g()), _b(c.b()), _a(c.a()) {}

  // Accessors
  uint8_t &r() { return _r; }
  const uint8_t &r() const { return _r; }

  uint8_t &g() { return _g; }
  const uint8_t &g() const { return _g; }

  uint8_t &b() { return _b; }
  const uint8_t &b() const { return _b; }

  uint8_t &a() { return _a; }
  const uint8_t &a() const { return _a; }

  int to_ansii() const
  {
    if (_r == 0 && _g == 0 && _b == 0)
      return 16;
    if (_r == _g && _g == _b)
    {
      if (_r < 8)
        return 16;
      if (_r > 248)
        return 231;
      return 232 + (_r - 8) / 10;
    }
    return 16 + 36 * (_r / 51) + 6 * (_g / 51) + (_b / 51);
  }

  static uint32_t to_hex(const Color &c) { return (c.r() << 16) | (c.g() << 8) | c.b(); }

  std::string to_ansii_fg_str() const
  {
    int code = to_ansii();
    return "\u001b[38;5;" + std::to_string(code) + "m";
  }

  std::string to_ansii_bg_str() const
  {
    int code = to_ansii();
    return "\u001b[48;5;" + std::to_string(code) + "m";
  }

  Color gray_scale() const
  {
    uint8_t gray = static_cast<uint8_t>(0.3 * _r + 0.59 * _g + 0.11 * _b);
    return Color(gray, gray, gray);
  }

  Color invert() const { return Color(255 - _r, 255 - _g, 255 - _b); }

  Color blend(const Color &c) const { return Color((_r + c.r()) / 2, (_g + c.g()) / 2, (_b + c.b()) / 2); }

  Color blend(const Color &c, float blend)
  {
    return Color(static_cast<uint8_t>(_r * (1 - blend) + c.r() * blend),
                 static_cast<uint8_t>(_g * (1 - blend) + c.g() * blend),
                 static_cast<uint8_t>(_b * (1 - blend) + c.b() * blend));
  }

  std::string get_rgb_string() const { return std::to_string(_r) + ", " + std::to_string(_g) + ", " + std::to_string(_b); }

  static Color get_random()
  {
    return Color(static_cast<uint8_t>(rand() % 256), static_cast<uint8_t>(rand() % 256), static_cast<uint8_t>(rand() % 256));
  }

  Color lerped(const Color &c, float t) const
  {
    return Color(_r + static_cast<uint8_t>((c.r() - _r) * t),
                 _g + static_cast<uint8_t>((c.g() - _g) * t),
                 _b + static_cast<uint8_t>((c.b() - _b) * t));
  }

  static Color lerp(const Color &a, const Color &b, float t)
  {
    return Color(a.r() + static_cast<uint8_t>((b.r() - a.r()) * t),
                 a.g() + static_cast<uint8_t>((b.g() - a.g()) * t),
                 a.b() + static_cast<uint8_t>((b.b() - a.b()) * t));
  }
};

#endif  // SIXTEEN_BIT_COLOR

#endif  // EIGHT_BIT_COLOR

std::vector<char> char_gradient = {'.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};

std::vector<Color> grayscale_gradient = {Color(utl::Color_codes::GRAY_2),
                                         Color(utl::Color_codes::GRAY_3),
                                         Color(utl::Color_codes::GRAY_4),
                                         Color(utl::Color_codes::GRAY_5),
                                         Color(utl::Color_codes::GRAY_6),
                                         Color(utl::Color_codes::GRAY_7),
                                         Color(utl::Color_codes::GRAY_8),
                                         Color(utl::Color_codes::GRAY_9),
                                         Color(utl::Color_codes::GRAY_10),
                                         Color(utl::Color_codes::GRAY_11),
                                         Color(utl::Color_codes::GRAY_12),
                                         Color(utl::Color_codes::GRAY_13),
                                         Color(utl::Color_codes::GRAY_14),
                                         Color(utl::Color_codes::GRAY_15),
                                         Color(utl::Color_codes::GRAY_16),
                                         Color(utl::Color_codes::GRAY_17),
                                         Color(utl::Color_codes::GRAY_18),
                                         Color(utl::Color_codes::GRAY_19),
                                         Color(utl::Color_codes::GRAY_20)};
std::vector<Color> heat_map = {
    Color(0x0000FF),  // Blue
    Color(0x0033FF),  // Intermediate between blue and next color
    Color(0x0066FF),
    Color(0x0099FF),
    Color(0x00CCFF),
    Color(0x00FFFF),  // Cyan
    Color(0x33FFCC),
    Color(0x66FF99),
    Color(0x99FF66),
    Color(0xCCFF33),
    Color(0xFFFF00),  // Yellow
    Color(0xFFCC00),
    Color(0xFF9900),
    Color(0xFF6600),
    Color(0xFF3300),
    Color(0xFF0000)  // Red
};

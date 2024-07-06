// Color  class with support for RGB hex strings and named colors

#include <cstdint>
#include <iostream>

// Only for terminal with 8 bit color support, so we will use  ifdef 8_BIT_COLOR and allow user to choose wether to use it or not

#ifdef EIGHT_BIT_COLOR

#define BLACK "\u001b[30m"
#define RED "\u001b[31m"
#define GREEN "\u001b[32m"
#define YELLOW "\u001b[33m"
#define BLUE "\u001b[34m"
#define MAGENTA "\u001b[35m"
#define CYAN "\u001b[36m"
#define WHITE "\u001b[37m"
#define RESET "\u001b[0m"
#define BOLD "\u001b[1m"
#define UNDERLINE "\u001b[4m"
#define REVERSED "\u001b[7m"
#define BG_BLACK "\u001b[40m"
#define BG_RED "\u001b[41m"
#define BG_GREEN "\u001b[42m"
#define BG_YELLOW "\u001b[43m"
#define BG_BLUE "\u001b[44m"
#define BG_MAGENTA "\u001b[45m"
#define BG_CYAN "\u001b[46m"
#define BG_WHITE "\u001b[47m"
#define BG_RESET "\u001b[49m"

#endif

#ifndef SIXTEEN_BIT_COLOR

#define SIXTEEN_BIT_COLOR

#define COUT_FG_CODE(i) "\u001b[38;5;" << i << "m"
#define COUT_BG_CODE(i) "\u001b[48;5;" << i << "m"
#define RESET "\u001b[0m"

class Color
{
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;

public:
    // Constructors 1. Default 2. RGB 3. Hex 4. ansii terminal code
    Color() : _r(0), _g(0), _b(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b) {}
    Color(const uint32_t &hex_val)
    {
        _r = (hex_val >> 16) & 0xFF;
        _g = (hex_val >> 8) & 0xFF;
        _b = (hex_val >> 0) & 0xFF;
    }

    // Accessors
    uint8_t &r() { return _r; }
    const uint8_t &r() const { return _r; }

    uint8_t &g() { return _g; }
    const uint8_t &g() const { return _g; }

    uint8_t &b() { return _b; }
    const uint8_t &b() const { return _b; }

    int to_ansii() const
    {
        if (_r == 0 && _g == 0 && _b == 0) return 16;
        if (_r == _g && _g == _b)
        {
            if (_r < 8) return 16;
            if (_r > 248) return 231;
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
};

#endif

int main()
{
    int red = 0xFF0000;
    Color c1(red);
    int blue = 0x0000FF;
    Color c2(blue);
    std::cout << COUT_BG_CODE(c2.to_ansii()) << COUT_FG_CODE(c1.to_ansii()) << "Heya" << RESET << std::endl;
}

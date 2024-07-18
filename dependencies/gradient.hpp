// We will implement gradient class
#include <algorithm>
#include <iostream>
#include <vector>

#include "color.hpp"

struct Color_stop
{
    float normal_pos;
    Color color;

    Color_stop() : normal_pos(0.0), color(Color()) {}

    Color_stop(float normal_pos, Color color) : normal_pos(normal_pos), color(color)
    {
        if (normal_pos < 0.0f || normal_pos > 1.0f)
            std::cerr << "Normal"
                         " ( "
                      << normal_pos << " ) position has to be in range [0.0f - 1.0f]";
    }
};

class Gradient
{
    std::vector<Color_stop> _color_stops;

public:
    Gradient() = default;
    Gradient(std::vector<Color_stop> stops) : _color_stops(stops) {}

    void add_color_stop(float normal_pos, Color color)
    {
        _color_stops.push_back(Color_stop(normal_pos, color));
        std::sort(
            _color_stops.begin(), _color_stops.end(), [](const Color_stop &a, const Color_stop &b) { return a.normal_pos < b.normal_pos; });
    }

    Color get_color_at(float t)
    {
        if (_color_stops.empty())
            return Color();
        if (t <= _color_stops.front().normal_pos)
            return _color_stops.front().color;
        if (t >= _color_stops.back().normal_pos)
            return _color_stops.back().color;

        for (size_t i = 1; i < _color_stops.size(); i++)
            if (t < _color_stops[i].normal_pos)
            {
                float blend = (t - _color_stops[i - 1].normal_pos) / (_color_stops[i].normal_pos - _color_stops[i - 1].normal_pos); 
                return _color_stops[i - 1].color.blend(_color_stops[i].color, blend);
            }
        return _color_stops.back().color;
    }
};

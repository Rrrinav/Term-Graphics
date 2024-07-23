// Camera for 2D games
#include <cstddef>

#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

class Camera2D
{
    size_t _screen_width;
    size_t _screen_height;
    utl::Vec<int, 2> _position;
    float _zoom;
    float _rotation;

public:
    Camera2D(size_t screen_width, size_t screen_height)
        : _screen_width(screen_width), _screen_height(screen_height), _position({0, 0}), _zoom(1.0f), _rotation(0.0f)
    {
    }
    Camera2D(size_t screen_width, size_t screen_height, utl::Vec<int, 2> position, float zoom, float rotation)
        : _screen_width(screen_width), _screen_height(screen_height), _position(position), _zoom(zoom), _rotation(rotation)
    {
    }
    Camera2D(size_t screen_width, size_t screen_height, utl::Vec<int, 2> position)
        : _screen_width(screen_width), _screen_height(screen_height), _position(position), _zoom(1.0f), _rotation(0.0f)
    {
    }
    Camera2D(size_t screen_width, size_t screen_height, float zoom)
        : _screen_width(screen_width), _screen_height(screen_height), _position({0, 0}), _zoom(zoom), _rotation(0.0f)
    {
    }
    Camera2D(const Camera2D &camera)
        : _screen_width(camera._screen_width),
          _screen_height(camera._screen_height),
          _position(camera._position),
          _zoom(camera._zoom),
          _rotation(camera._rotation)
    {
    }

    void set_position(utl::Vec<int, 2> position) { _position = position; }
    void set_zoom(float zoom) { _zoom = zoom; }
    void zoom_by(float dz) { _zoom += dz; }
    void set_rotation(float rotation) { _rotation = rotation; }
    void rotate_by(float dr) { _rotation += dr; }
    void panf(utl::Vec<float, 2> dp) { _position = _position + dp; }
    void pan(utl::Vec<int, 2> dp) { _position = _position + dp; }
    utl::Vec<int, 2> get_position() const { return _position; }
    float get_zoom() const { return _zoom; }
    float get_rotation() const { return _rotation; }
    utl::Vec<int, 2> world_to_screen(utl::Vec<int, 2> world_position) const
    {
        int centerx = static_cast<int>(_screen_width / 2);
        int centery = static_cast<int>(_screen_height / 2);
        int x = static_cast<int>((world_position[0] - _position[0]) * _zoom + (int)(centerx));
        int y = static_cast<int>((world_position[1] - _position[1]) * _zoom + (int)(centery));
        utl::Vec<int, 2> result = utl::Vec<int, 2>{x, y}.rotate_about_center({(int)centerx, centery}, _rotation, 'z');
        return result;
    }

    utl::Vec<int, 2> world_to_screen_no_rotation(utl::Vec<int, 2> world_position) const
    {
        int centerx = static_cast<int>(_screen_width / 2);
        int centery = static_cast<int>(_screen_height / 2);
        int x = static_cast<int>((world_position[0] - _position[0]) * _zoom + (int)(centerx));
        int y = static_cast<int>((world_position[1] - _position[1]) * _zoom + (int)(centery));
        return utl::Vec<int, 2>{x, y};
    }

    utl::Vec<int, 2> screen_to_world(utl::Vec<int, 2> screen_position) const
    {
        int centerx = static_cast<int>(_screen_width / 2);
        int centery = static_cast<int>(_screen_height / 2);
        utl::Vec<int, 2> result = screen_position.rotate_about_center({centerx, centery}, -_rotation, 'z');
        int x = static_cast<int>((result[0] - centerx) / _zoom + _position[0]);
        int y = static_cast<int>((result[1] - centery) / _zoom + _position[1]);
        return utl::Vec<int, 2>{x, y};
    }

    utl::Vec<int, 2> screen_to_world_no_rotation(utl::Vec<int, 2> screen_position) const
    {
        int centerx = static_cast<int>(_screen_width / 2);
        int centery = static_cast<int>(_screen_height / 2);
        int x = static_cast<int>((screen_position[0] - centerx) / _zoom + _position[0]);
        int y = static_cast<int>((screen_position[1] - centery) / _zoom + _position[1]);
        return utl::Vec<int, 2>{x, y};
    }
};

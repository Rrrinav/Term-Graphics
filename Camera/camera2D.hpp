#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>

#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

class Camera2D
{
  size_t _screen_width;
  size_t _screen_height;
  utl::Vec<float, 2> _position;
  float _zoom;
  float _rotation;

  utl::Vec<float, 2> _world_min;
  utl::Vec<float, 2> _world_max;
  float _min_zoom;
  float _max_zoom;

  public:
  Camera2D(size_t screen_width, size_t screen_height)
      : _screen_width(screen_width),
        _screen_height(screen_height),
        _position({0, 0}),
        _zoom(1.0f),
        _rotation(0.0f),
        _world_min({-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()}),
        _world_max({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}),
        _min_zoom(0.1f),
        _max_zoom(10.0f)
  {
  }

  Camera2D(size_t screen_width, size_t screen_height, utl::Vec<float, 2> position, float zoom, float rotation)
      : _screen_width(screen_width),
        _screen_height(screen_height),
        _position(position),
        _zoom(zoom),
        _rotation(rotation),
        _world_min({-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()}),
        _world_max({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}),
        _min_zoom(0.1f),
        _max_zoom(10.0f)
  {
  }

  Camera2D(size_t screen_width, size_t screen_height, utl::Vec<float, 2> position)
      : _screen_width(screen_width),
        _screen_height(screen_height),
        _position(position),
        _zoom(1.0f),
        _rotation(0.0f),
        _world_min({-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()}),
        _world_max({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}),
        _min_zoom(0.1f),
        _max_zoom(10.0f)
  {
  }

  Camera2D(size_t screen_width, size_t screen_height, float zoom)
      : _screen_width(screen_width),
        _screen_height(screen_height),
        _position({0, 0}),
        _zoom(zoom),
        _rotation(0.0f),
        _world_min({-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()}),
        _world_max({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}),
        _min_zoom(0.1f),
        _max_zoom(10.0f)
  {
  }

  Camera2D(const Camera2D &camera)
      : _screen_width(camera._screen_width),
        _screen_height(camera._screen_height),
        _position(camera._position),
        _zoom(camera._zoom),
        _rotation(camera._rotation),
        _world_min(camera._world_min),
        _world_max(camera._world_max),
        _min_zoom(camera._min_zoom),
        _max_zoom(camera._max_zoom)
  {
  }

  void set_position(utl::Vec<int, 2> position)
  {
    _position = position;
    clamp_position();
  }

  void set_zoom(float zoom) { _zoom = std::clamp(zoom, _min_zoom, _max_zoom); }

  void zoom_by(float dz) { set_zoom(_zoom + dz); }

  void set_rotation(float rotation) { _rotation = rotation; }

  void rotate_by(float dr) { _rotation += dr; }

  void panf(utl::Vec<float, 2> dp, float delta_time)
  {
    _position = _position + dp * delta_time;
    clamp_position();
  }

  void pan(utl::Vec<float, 2> dp)
  {
    _position = _position + dp;
    clamp_position();
  }

  utl::Vec<float, 2> get_position() const { return _position; }

  float get_zoom() const { return _zoom; }

  float get_rotation() const { return _rotation; }

  void set_world_bounds(utl::Vec<int, 2> min, utl::Vec<int, 2> max)
  {
    _world_min = min;
    _world_max = max;
    clamp_position();
  }

  void set_zoom_limits(float min_zoom, float max_zoom)
  {
    _min_zoom = min_zoom;
    _max_zoom = max_zoom;
    set_zoom(_zoom);  // Apply zoom limits immediately
  }

  void set_screen_size(size_t screen_width, size_t screen_height)
  {
    _screen_width = screen_width;
    _screen_height = screen_height;
  }

  void reset()
  {
    _position = {0, 0};
    _zoom = 1.0f;
    _rotation = 0.0f;
  }

  void follow(utl::Vec<float, 2> target_position, float delta_time, float follow_speed)
  {
    if (delta_time == 0.0f)
      return;

    // Compute the direction vector
    utl::Vec<float, 2> direction = target_position - _position;
    float distance = direction.magnitude();

    // Calculate the actual move distance based on speed and delta_time
    float move_distance = follow_speed * delta_time;

    // If we're close enough to the target, just set the position to the target
    if (distance < move_distance)
    {
      _position = target_position;
    }
    else
    {
      // Normalize the direction and move by the computed move distance
      direction.normalize();
      // _position = (_position + (direction * move_distance));
      _position[0] += direction[0] * move_distance;
      _position[1] += direction[1] * move_distance;
    }

    // Ensure the position is within the world bounds
    clamp_position();
  }
  void smooth_follow(utl::Vec<float, 2> target_position, float delta_time, float smooth_factor)
  {
    if (delta_time == 0.0f || smooth_factor == 0.0f)
      return;

    // Linear interpolation towards the target position
    _position = _position + (target_position - _position) * smooth_factor * delta_time;

    clamp_position();
  }
  utl::Vec<int, 2> world_to_screen(utl::Vec<float, 2> world_position) const
  {
    int centerx = static_cast<int>(_screen_width / 2);
    int centery = static_cast<int>(_screen_height / 2);
    int x = static_cast<int>((world_position[0] - _position[0]) * _zoom + centerx);
    int y = static_cast<int>((world_position[1] - _position[1]) * _zoom + centery);
    return utl::Vec<int, 2>{x, y}.rotate_about_center({centerx, centery}, _rotation, 'z');
  }

  utl::Vec<int, 2> world_to_screen_no_rotation(utl::Vec<float, 2> world_position) const
  {
    int centerx = static_cast<int>(_screen_width / 2);
    int centery = static_cast<int>(_screen_height / 2);
    int x = static_cast<int>((world_position[0] - _position[0]) * _zoom + centerx);
    int y = static_cast<int>((world_position[1] - _position[1]) * _zoom + centery);
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

  private:
  void clamp_position()
  {
    _position[0] = std::clamp(_position[0], _world_min[0], _world_max[0]);
    _position[1] = std::clamp(_position[1], _world_min[1], _world_max[1]);
  }
};

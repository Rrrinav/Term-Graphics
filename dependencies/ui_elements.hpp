//  UI elements class to manage all the UI elements like buttons, textboxes, etc.
#include <cstddef>
#include <functional>
#include <memory>

#include "../window/window.hpp"
#include "color.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

class UI_element
{
protected:
    utl::Vec<int, 2> _position;
    size_t _width;
    size_t _height;
    char _fill_char;
    Color _bg_color;
    Color _fg_color;

public:
    UI_element() = default;
    UI_element(utl::Vec<int, 2> pos, size_t w, size_t h, char ch, Color bg, Color fg)
        : _position(pos), _width(w), _height(h), _fill_char(ch), _bg_color(bg), _fg_color(fg)
    {
    }
    virtual ~UI_element() = default;
    virtual utl::Vec<int, 2> position() const { return _position; }
    virtual size_t width() const { return _width; }
    virtual size_t height() const { return _height; }
    virtual char fill_char() const { return _fill_char; }
    virtual Color bg_color() const { return _bg_color; }
    virtual Color fg_color() const { return _fg_color; }
    virtual void handle_event(const Mouse_event &event) = 0;
};

class Button : public UI_element
{
    std::string _label;
    std::function<void()> _callback;
    bool _is_pressed = false;
    bool _is_hovered = false;

public:
    Button() = default;
    Button(utl::Vec<int, 2> pos, size_t w, size_t h, char ch, Color bg, Color fg, std::string lb, std::function<void()> cb)
        : UI_element(pos, w, h, ch, bg, fg), _label(lb), _callback(cb)
    {
    }
    ~Button() = default;

    std::string label() const { return _label; }
    std::function<void()> callback() const { return _callback; }
    bool is_pressed() const { return _is_pressed; }
    void handle_event(const Mouse_event &event) override
    {
        if (event.x >= _position[0] && event.x <= _position[0] + _width && event.y >= _position[1] && event.y <= _position[1] + _height)
        {
            _is_hovered = true;
            if (event.event == Mouse_event_type::LEFT_CLICK && !_is_pressed)
            {
                _is_pressed = true;
                _callback();
            }
            else if (event.event == Mouse_event_type::LEFT_RELEASE)
                _is_pressed = false;
            else
                _is_pressed = false;
        }
        else
        {
            _is_hovered = false;
            _is_pressed = false;
        }
    }
};

class Slider : public UI_element
{
    // BUG: width changing when clicking on the slider probably renderer issue
    float _min_value;
    float _max_value;
    float _value;
    std::function<void(float)> _callback;
    bool _is_pressed = false;
    bool _is_hovered = false;
    bool _is_toggle_hovered = false;

public:
    Slider() = default;
    Slider(utl::Vec<int, 2> pos, size_t w, char ch, Color bg, Color fg, float min_val, float max_val, float init_val,
           std::function<void(float)> cb)
        : UI_element(pos, w, 1, ch, bg, fg),

          _min_value(min_val),
          _max_value(max_val),
          _value(init_val),
          _callback(cb)
    {
    }

    float value() const { return _value; }

    void handle_event(const Mouse_event &event) override
    {
        if (event.x >= _position[0] && event.x <= _position[0] + _width && event.y >= _position[1] && event.y <= _position[1] + _height)
        {
            _is_hovered = true;
            if (event.event == Mouse_event_type::LEFT_CLICK)
            {
                _is_pressed = true;
                _value = _min_value + (static_cast<float>(event.x - _position[0]) / _width) * (_max_value - _min_value);
                _callback(_value);
            }
            else if (event.event == Mouse_event_type::LEFT_RELEASE)
            {
                _is_pressed = false;
            }
        }
        else
        {
            _is_hovered = false;
            _is_pressed = false;
        }
    }
};

class UI_manager : public Window
{
    std::vector<std::shared_ptr<UI_element>> elements;

public:
    UI_manager() = default;
    void add_element(std::shared_ptr<UI_element> element) { elements.push_back(element); }
    void handle_events()
    {
        for (auto &element : elements)
        {
            Window::update_input_states();
            Mouse_event event = Window::get_mouse_event();
            element->handle_event(event);
        }
    }
};

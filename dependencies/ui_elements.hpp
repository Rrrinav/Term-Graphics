//  UI elements class to manage all the UI elements like buttons, textboxes, etc.
#include <cstddef>
#include <functional>
#include <memory>

#include "../window/window.hpp"
#include "color.hpp"
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

// TODO: Implement all the getters and setters...!

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
    bool is_hovered() const { return _is_hovered; }
    void set_bg_color(Color bg) { _bg_color = bg; }
    void handle_event(const Mouse_event &event) override
    {
        if (event.x >= _position[0] && event.x <= _position[0] + (int) _width && event.y >= _position[1] && event.y <= _position[1] + (int) _height)
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
    float _min_value = 0.0f;
    float _max_value = 1.0f;
    float _value = 0.0f;
    std::function<void(float)> _callback;
    bool _is_pressed = false;
    bool _is_hovered = false;
    bool _is_toggle_hovered = false;

public:
    Slider() = default;
    Slider(utl::Vec<int, 2> pos, size_t w, char ch, Color bg, Color fg, std::function<void(float)> cb)
        : UI_element(pos, w, 1, ch, bg, fg), _callback(cb)
    {
    }

    float value() const { return _value; }
    void set_value(float val) { _value = val; }

    void handle_event(const Mouse_event &event) override
    {
        utl::Vec<int, 2> toggle_pos = {static_cast<int>(_position[0] + _value * _width), _position[1]};
        if (event.x >= _position[0] && event.x <= _position[0] +  (int)_width && event.y >= _position[1] && event.y <= _position[1] +  (int)_height)
        {
            _is_hovered = true;
            if (event.x == toggle_pos[0] && event.y == toggle_pos[1])
            {
                _is_toggle_hovered = true;
                if (event.event == Mouse_event_type::LEFT_CLICK)
                    _is_pressed = true;
            }

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

        // Handle dragging
        if (_is_pressed && event.event == Mouse_event_type::MOUSE_MOVE)
        {
            float new_value = _min_value + (static_cast<float>(event.x - _position[0]) / _width) * (_max_value - _min_value);
            _value = new_value;
            _callback(_value);
        }
    }
};

class Textbox : public UI_element
{
    std::string _text;
    bool _is_active = false;

public:
    Textbox() = default;
    Textbox(utl::Vec<int, 2> position, size_t w, size_t h, char ch, Color bg, Color fg, std::string txt)
        : UI_element(position, w, h, ch, bg, fg), _text(txt)
    {
    }
    std::string text() const { return _text; }
    void set_text(std::string txt) { _text = txt; }
    bool is_active() const { return _is_active; }
    void handle_event(const Mouse_event &event) override
    {
        if (event.x >= _position[0] && event.x <= _position[0] +  (int)_width && event.y >= _position[1] && event.y <= _position[1] +  (int)_height)
        {
            if (event.event == Mouse_event_type::LEFT_CLICK)
                _is_active = true;
        }
        else if (event.event == Mouse_event_type::LEFT_CLICK)
            _is_active = false;

        handle_key_event();
    }

    void handle_key_event()
    {
        if (_is_active)
        {
            Keys key = Window::check_input();
            if (key == Keys::KEY_BACKSPACE)
            {
                if (_text.size() > 0)
                    _text.pop_back();
            }
            else if (key == Keys::KEY_ENTER)
                _text += '\n';
            else if (key == Keys::KEY_SPACE)
                _text += ' ';
            else if (key == Keys::KEY_UNKNOWN)
                return;
            else
                _text += Window::to_char(key);
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

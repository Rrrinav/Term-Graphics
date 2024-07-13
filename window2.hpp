#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#define L_GEBRA_IMPLEMENTATION
#include "keys.hpp"
#include "l_gebra.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#endif

enum Event_type
{
    RIGHT_CLICK,
    LEFT_CLICK,
    RELEASE,
    SCROLL_UP,
    SCROLL_DOWN,
    MIDDLE_CLICK,
    MOUSE_MOVE,
};

struct Mouse_event
{
    int x;
    int y;
    Event_type event;
};

class Window
{
private:
    static std::unordered_map<Keys, bool> key_states;
    static utl::Vec<int, 2> mouse_pos;
    static bool mouse_moved;
    static Mouse_event mouse_event;

#ifdef _WIN32
    HANDLE hConsole;
    HANDLE hConsoleInput;
#else
    struct termios orig_termios;
#endif

public:
    Window() { init_terminal(); }
    ~Window() { cleanup_terminal(); }

    void init_terminal()
    {
#ifdef _WIN32
        // Windows implementation remains the same
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
        setConsoleMode(hConsoleInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
        system("cls");
#else
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        std::cout << "\033[?1003h";  // Enable mouse tracking
        std::cout << "\033[?1006h";  // Enable SGR (1006) mouse mode
        std::cout << "\033%G";       // Enable UTF-8 input mode
        system("clear");
#endif
    }

    void cleanup_terminal()
    {
#ifdef _WIN32
        CloseHandle(hConsole);
#else
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        std::cout << "\033[?1003l";  // Disable mouse tracking
        std::cout << "\033[?1006l";  // Disable SGR (1006) mouse mode
        std::cout << "\033%@";       // Disable UTF-8 input mode
#endif
        system("clear");
    }

    void draw(const std::string &output)
    {
#ifdef _WIN32
        DWORD written;
        WriteConsoleOutputCharacter(hConsole, output.c_str(), output.length(), {0, 0}, &written);
#else
        std::cout << "\033[H" << output << std::flush;
#endif
    }

    static int parse_key(int key)
    {
        if (key >= 'a' && key <= 'z') return KEY_a + (key - 'a');  // Lowercase letters
        if (key >= 'A' && key <= 'Z') return KEY_A + (key - 'A');  // Uppercase letters
        if (key >= '0' && key <= '9') return KEY_0 + (key - '0');  // Numbers

        switch (key)
        {
            case '\n':
                return KEY_ENTER;
            case ' ':
                return KEY_SPACE;
            case 27:
                return KEY_ESC;  // Escape key
            case '\t':
                return KEY_TAB;  // Tab key
            case 127:
                return KEY_BACKSPACE;  // Backspace key
            default:
                if (key >= 1 && key <= 26) return KEY_a + (key - 1);       // Ctrl + [a-z]
                if (key >= 129 && key <= 154) return KEY_A + (key - 129);  // Ctrl + [A-Z]
                if (key >= 59 && key <= 68) return KEY_F1 + (key - 59);    // F1-F10
                return KEY_UNKNOWN;
        }
    }

    static Keys check_input()
    {
#ifdef _WIN32
        // Windows implementation remains the same
        // ...
#else
        struct pollfd fds[1];
        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;

        int ret = poll(fds, 1, 0);  // Timeout of 0 for non-blocking

        if (ret > 0 && (fds[0].revents & POLLIN))
        {
            char key;
            read(STDIN_FILENO, &key, 1);
            return static_cast<Keys>(parse_key(key));
        }
#endif
        return Keys::KEY_UNKNOWN;
    }

    static utl::Vec<int, 2> get_mouse_pos() { return mouse_pos; }

    static bool has_mouse_moved() { return mouse_moved; }

    static Mouse_event get_mouse_event()
    {
        mouse_moved = false;
        return mouse_event;
    }

    static bool is_pressed(Keys key)
    {
        auto it = key_states.find(key);
        return (it != key_states.end() && it->second);
    }

    static bool is_not_pressed(Keys key)
    {
        auto it = key_states.find(key);
        return (it == key_states.end() || !it->second);
    }

    static int get_key_state(Keys key)
    {
#ifdef _WIN32
        SHORT state = GetAsyncKeyState(key);
        return (state & 0x8000) ? key : 0;
#else
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF)
        {
            return (parse_key(static_cast<int>(ch)));
        }
        else
        {
            return 0;
        }
#endif
    }

    static void update_input_states()
    {
#ifdef _WIN32
        // Windows implementation remains the same
        // ...
#else
        fd_set readfds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Reset all key states to false at the start of each frame
        for (auto &pair : key_states)
        {
            pair.second = false;
        }

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
        {
            char buf[1024];
            ssize_t nread = read(STDIN_FILENO, buf, sizeof(buf));
            if (nread > 0)
            {
                for (ssize_t i = 0; i < nread; ++i)
                {
                    if (buf[i] == '\033' && i + 2 < nread && buf[i + 1] == '[' && buf[i + 2] == '<')
                    {
                        // SGR mouse event
                        int button = 0, x = 0, y = 0;
                        char eventType = 0;
                        int parsed = 0;
                        int j = i + 3;  // Start after "<"

                        // Parse button
                        while (j < nread && buf[j] >= '0' && buf[j] <= '9')
                        {
                            button = button * 10 + (buf[j] - '0');
                            j++;
                        }
                        if (j < nread && buf[j] == ';') j++;

                        // Parse x
                        while (j < nread && buf[j] >= '0' && buf[j] <= '9')
                        {
                            x = x * 10 + (buf[j] - '0');
                            j++;
                        }
                        x /= 2;
                        if (j < nread && buf[j] == ';') j++;

                        // Parse y
                        while (j < nread && buf[j] >= '0' && buf[j] <= '9')
                        {
                            y = y * 10 + (buf[j] - '0');
                            j++;
                        }

                        // Get event type
                        if (j < nread)
                        {
                            eventType = buf[j];
                            parsed = j - i + 1;
                        }

                        if (parsed > 0 && (eventType == 'M' || eventType == 'm'))
                        {
                            mouse_pos = {x - 1, y - 1};  // SGR reports 1-based coordinates
                            mouse_event.x = x - 1;
                            mouse_event.y = y - 1;

                            if (mouse_pos[0] != x - 1 || mouse_pos[1] != y - 1)
                            {
                                mouse_moved = true;
                            }

                            switch (button)
                            {
                                case 0:
                                    mouse_event.event = Event_type::LEFT_CLICK;
                                    break;
                                case 1:
                                    mouse_event.event = Event_type::MIDDLE_CLICK;
                                    break;
                                case 2:
                                    mouse_event.event = Event_type::RIGHT_CLICK;
                                    break;
                                case 64:
                                    mouse_event.event = Event_type::SCROLL_UP;
                                    break;
                                case 65:
                                    mouse_event.event = Event_type::SCROLL_DOWN;
                                    break;
                                case 32:
                                    mouse_event.event = Event_type::MOUSE_MOVE;
                                    break;
                                default:
                                    if (eventType == 'm')
                                        mouse_event.event = Event_type::RELEASE;
                                    else
                                        mouse_event.event = Event_type::MOUSE_MOVE;
                                    break;
                            }

                            i += parsed - 1;  // Move i to the end of the parsed sequence
                        }
                    }
                    else
                    {
                        Keys k = static_cast<Keys>(parse_key(buf[i]));
                        key_states[k] = true;
                    }
                }
            }
        }
#endif
    }
};

// Initialize the static members
std::unordered_map<Keys, bool> Window::key_states;
utl::Vec<int, 2> Window::mouse_pos;
bool Window::mouse_moved;
Mouse_event Window::mouse_event;

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

/**
 * Enum representing different types of mouse events.
 */
enum Event_type
{
    RIGHT_CLICK,
    LEFT_CLICK,
    RELEASE,
    SCROLL_UP,
    SCROLL_DOWN,
    MIDDLE_CLICK,
};

/**
 * Struct representing a mouse event with its position and type.
 */
struct Mouse_event
{
    int x;             ///< X position of the mouse event
    int y;             ///< Y position of the mouse event
    Event_type event;  ///< Type of the mouse event
};

/**
 * Class representing a terminal window for input and output handling.
 */
class Window
{
private:
    static std::unordered_map<Keys, bool> key_states;  ///< Map of key states
    static utl::Vec<int, 2> mouse_pos;                 ///< Mouse position
    static bool mouse_moved;                           ///< Flag to check if the mouse has moved
    static Mouse_event mouse_event;                    ///< Last mouse event

#ifdef _WIN32
    HANDLE hConsole;       ///< Console handle for Windows
    HANDLE hConsoleInput;  ///< Console input handle for Windows
#else
    struct termios orig_termios;  ///< Original terminal settings for Unix
#endif

public:
    /**
     * Constructor to initialize the terminal.
     */
    Window() { init_terminal(); }

    /**
     * Destructor to cleanup the terminal.
     */
    ~Window() { cleanup_terminal(); }

    /**
     * Initialize the terminal for raw input mode.
     */
    void init_terminal()
    {
#ifdef _WIN32
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
        setConsoleMode(hConsoleInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
        system("cls");  // or "cls" on Windows
#else
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        std::cout << "\033[?1003h";  // Enable mouse tracking
        system("clear");             // or "cls" on Windows
#endif
    }

    /**
     * Cleanup the terminal, restoring original settings.
     */
    void cleanup_terminal()
    {
#ifdef _WIN32
        CloseHandle(hConsole);
#else
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        std::cout << "\033[?1003l";  // Disable mouse tracking
#endif
        system("clear");  // or "cls" on Windows
    }

    /**
     * Draw the given output string to the terminal.
     * @param output The string to draw to the terminal.
     */
    void draw(const std::string &output)
    {
#ifdef _WIN32
        DWORD written;
        WriteConsoleOutputCharacter(hConsole, output.c_str(), output.length(), {0, 0}, &written);
#else
        std::cout << "\033[H" << output << std::flush;
#endif
    }

    /**
     * Parse the given key code and return the corresponding enum value.
     * @param key The key code to parse.
     * @return The corresponding Keys enum value.
     */
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

    /**
     * Check for any input and return the corresponding key code.
     * @return The Keys enum value of the input, or KEY_UNKNOWN if no input is detected.
     */
    static Keys check_input()
    {
#ifdef _WIN32
        INPUT_RECORD inputRecord;
        DWORD numEvents;
        DWORD numRead;

        GetNumberOfConsoleInputEvents(hConsoleInput, &numEvents);
        if (numEvents != 0)
        {
            ReadConsoleInput(hConsoleInput, &inputRecord, 1, &numRead);
            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
            {
                char key = inputRecord.Event.KeyEvent.uChar.AsciiChar;
                return static_cast<Keys>(parse_key(key));
            }
        }
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

    /**
     * Get the current mouse position.
     * @return A vector representing the current mouse position.
     */
    static utl::Vec<int, 2> get_mouse_pos()
    {
        static utl::Vec<int, 2> last_valid_pos = {-1, -1};  // Store last valid position

#ifdef _WIN32
        DWORD numEvents = 0;
        GetNumberOfConsoleInputEvents(hConsoleInput, &numEvents);

        if (numEvents > 0)
        {
            INPUT_RECORD inputBuffer[128];
            DWORD eventsRead;

            if (ReadConsoleInput(hConsoleInput, inputBuffer, std::min(numEvents, DWORD(128)), &eventsRead))
            {
                for (DWORD i = 0; i < eventsRead; i++)
                {
                    if (inputBuffer[i].EventType == MOUSE_EVENT)
                    {
                        MOUSE_EVENT_RECORD mouseEvent = inputBuffer[i].Event.MouseEvent;
                        if (mouseEvent.dwEventFlags == MOUSE_MOVED || (mouseEvent.dwEventFlags == 0 && mouseEvent.dwButtonState != 0))
                        {
                            last_valid_pos[0] = mouseEvent.dwMousePosition.X;
                            last_valid_pos[1] = mouseEvent.dwMousePosition.Y;
                        }
                    }
                }
            }
        }
#else
        //  This was the original code for mouse input on Unix systems
        //  It was replaced by the code below it because it was not working properly as we need to update the mouse position every frame
        //  along with the key states.
        //  This way only one event is recorded per frame but doing it to
        // struct pollfd fds[2];
        // fds[0].fd = STDIN_FILENO;
        // fds[0].events = POLLIN;
        //
        // if (poll(fds, 1, 0) > 0 && (fds[0].revents & POLLIN))
        // {
        //     char buf[64];
        //     ssize_t nread = read(STDIN_FILENO, buf, sizeof(buf));
        //
        //     for (ssize_t i = 0; i < nread; i++)
        //     {
        //         if (buf[i] == '\033' && i + 5 < nread && buf[i + 1] == '[' && buf[i + 2] == 'M')
        //         {
        //             int button = buf[i + 3] - 32;
        //             std::cout << "Button: " << button << std::endl;
        //             int x = buf[i + 4] - 33;
        //             std::cout << "X: " << x << std::endl;
        //             int y = buf[i + 5] - 33;
        //             std::cout << "Y: " << y << std::endl;
        //             // Check if it's a movement or button press event
        //             last_valid_pos[0] = x;
        //             last_valid_pos[1] = y;
        //
        //             i += 5;  // Skip the rest of this mouse event
        //         }
        //     }
        // }
        return mouse_pos;
#endif

        return mouse_pos;
    }

    /**
     * Check if the mouse has moved.
     * @return True if the mouse has moved, otherwise false.
     */
    static bool has_mouse_moved() { return mouse_moved; }

    /**
     * Get the last mouse event.
     * @return The last Mouse_event.
     */
    static Mouse_event get_mouse_event()
    {
        mouse_moved = false;
        return mouse_event;
    }

    /**
     * Check if the specified key is currently pressed.
     * @param key The key to check.
     * @return True if the key is pressed, otherwise false.
     */
    static bool is_pressed(Keys key)
    {
        auto it = key_states.find(key);
        return (it != key_states.end() && it->second);
    }

    /**
     * Check if the specified key is not currently pressed.
     * @param key The key to check.
     * @return True if the key is not pressed, otherwise false.
     */
    static bool is_not_pressed(Keys key)
    {
        auto it = key_states.find(key);
        return (it == key_states.end() || !it->second);
    }

    /**
     * Get the state of the specified key.
     * @param key The key to check.
     * @return The key state as an integer.
     */
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

    /**
     * Update the states of all keys and the mouse position.
     */
    static void update_input_states()
    {
#ifdef _WIN32
        for (int key = 0; key < 256; ++key)
        {
            SHORT state = GetAsyncKeyState(key);
            key_states[static_cast<Keys>(key)] = (state & 0x8000) != 0;
        }
#else
        fd_set readfds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        utl::Vec<int, 2> last_valid_pos = {-1, -1};  // Store last valid position
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Reset all key states to false at the start of each frame
        for (auto &pair : key_states)
        {
            pair.second = false;
        }

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
        {
            unsigned char buf[256];
            ssize_t nread = read(STDIN_FILENO, buf, sizeof(buf));
            if (nread > 0)
            {
                for (ssize_t i = 0; i < nread; ++i)
                {
                    if (buf[i] == '\033' && i + 2 < nread && buf[i + 1] == '[' && buf[i + 2] == 'M')
                    {
                        int button = buf[i + 3] - 32;
                        int x = buf[i + 4] - 33;
                        int y = buf[i + 5] - 33;
                        if (mouse_pos[0] != x || mouse_pos[1] != y)
                        {
                            mouse_moved = true;
                        }
                        mouse_pos = {x, y};
                        mouse_event.x = x / 2;
                        mouse_event.y = y;
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
                            case 3:
                                mouse_event.event = Event_type::RELEASE;
                                break;
                            default:
                                mouse_event.event = Event_type::RELEASE;
                                break;
                        }
                        i += 5;  // Skip the entire mouse event sequence
                    }
                    Keys k = static_cast<Keys>(parse_key(buf[i]));
                    key_states[k] = true;
                    //    std::cout << "Key pressed: " << static_cast<int>(k) << std::endl;
                }
            }
        }
#endif
    }
};

// Initialize the static member
std::unordered_map<Keys, bool> Window::key_states;
utl::Vec<int, 2> Window::mouse_pos;
bool Window::mouse_moved;
Mouse_event Window::mouse_event;

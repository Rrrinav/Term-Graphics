#pragma once
#include <fcntl.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"
#include "keys.hpp"
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
enum Mouse_event_type
{
    RIGHT_CLICK,
    LEFT_CLICK,
    MIDDLE_CLICK,
    RIGHT_RELEASE,
    LEFT_RELEASE,
    MIDDLE_RELEASE,
    SCROLL_UP,
    SCROLL_DOWN,
    MOUSE_MOVE,
};

/**
 * Struct representing a mouse event with its position and type.
 */
struct Mouse_event
{
    int x;                   ///< X position of the mouse event
    int y;                   ///< Y position of the mouse event
    Mouse_event_type event;  ///< Type of the mouse event
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

    /**e. If you'd like to tell me about your library, I'd be happy to discuss it or help with any questions you have.
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
        std::cout << "\033[?1000h";  // Enable xterm mouse reporting
        std::cout << "\033[?1002h";  // Enable UTF-8 mouse
        std::cout << "\033[?1005h";  // Enable urxvt mouse
        std::cout << "\033[?1015h";  // Enable utf8 ext mode
        std::cout << "\033[?1003h";  // Enable all mouse tracking
        std::cout << "\033[?1006h";  // Enable SGR mouse mode
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

        std::cout << "\033[?1000l";  // Enable xterm mouse reporting
        std::cout << "\033[?1002l";  // Enable UTF-8 mouse
        std::cout << "\033[?1005l";  // Enable urxvt mouse
        std::cout << "\033[?1015l";  // Enable utf8 ext mode
        std::cout << "\033[?1003l";  // Enable all mouse tracking
        std::cout << "\033[?1006l";  // Disable mouse tracking
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
        if (key >= 'a' && key <= 'z')
            return KEY_a + (key - 'a');  // Lowercase letters
        if (key >= 'A' && key <= 'Z')
            return KEY_A + (key - 'A');  // Uppercase letters
        if (key >= '0' && key <= '9')
            return KEY_0 + (key - '0');  // Numbers
        else if (key < 27)               // Ctrl+A to Ctrl+Z or Ctrl+a to Ctrl+z
        {
            Keys k_upper = static_cast<Keys>(KEY_Ctrl_A + (key - 1));
            Keys k_lower = static_cast<Keys>(KEY_Ctrl_a + (key - 1));
            // key_states[k_upper] = true;
            // key_states[k_lower] = true;  // Set both upper and lower case
        }
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
                if (key >= 1 && key <= 26)
                    return KEY_a + (key - 1);  // Ctrl + [a-z]
                if (key >= 129 && key <= 154)
                    return KEY_A + (key - 129);  // Ctrl + [A-Z]
                if (key >= 59 && key <= 68)
                    return KEY_F1 + (key - 59);  // F1-F10
                return KEY_UNKNOWN;
        }
    }

    /**
     * Take the Key and return corresponding ascii value (char)
     * @param key The key to parse.
     * @return The correspoding ascii value (char KEY_w => 'w')
     */
    static char to_char(Keys key)
    {
        if (key >= KEY_a && key <= KEY_z)
            return 'a' + (key - KEY_a);
        if (key >= KEY_A && key <= KEY_Z)
            return 'A' + (key - KEY_A);
        if (key >= KEY_0 && key <= KEY_9)
            return '0' + (key - KEY_0);
        switch (key)
        {
            case KEY_ENTER:
                return '\n';
            case KEY_SPACE:
                return ' ';
            case KEY_TAB:
                return '\t';
            case KEY_ESC:
                return 27;
            case KEY_BACKSPACE:
                return 127;
            default:
                return '\0';
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
        for (auto &pair : key_states)
            if (pair.second)
                return pair.first;
#endif
        return Keys::KEY_UNKNOWN;
    }

    /**
     * Get the current mouse position.
     * @return A vector representing the current mouse position.
     */
    static utl::Vec<int, 2> get_mouse_pos() { return mouse_pos; }

    /**
     * Check if the mouse has moved.e. If you'd like to tell me about your library, I'd be happy to discuss it or help with any questions you have.
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
            return (parse_key(static_cast<int>(ch)));
        else
            return 0;
#endif
    }

    static void update_mouse_and_key_states()
    {
#ifdef _WIN32
        DWORD numRead;
        INPUT_RECORD inputRecord;
        while (true)
        {
            // Check if there are any input events
            if (!GetNumberOfConsoleInputEvents(hConsoleInput, &numRead) || numRead == 0)
                break;

            // Read the input event
            ReadConsoleInput(hConsoleInput, &inputRecord, 1, &numRead);
            if (inputRecord.EventType == KEY_EVENT)
            {
                char key = inputRecord.Event.KeyEvent.uChar.AsciiChar;
                Keys parsedKey = static_cast<Keys>(parse_key(key));
                key_states[parsedKey] = inputRecord.Event.KeyEvent.bKeyDown;
            }
            else if (inputRecord.EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
                // Update mouse position
                mouse_pos[0] = mouseEvent.dwMousePosition.X;
                mouse_pos[1] = mouseEvent.dwMousePosition.Y;

                // Update mouse event type
                if (mouseEvent.dwEventFlags & MOUSE_MOVED)
                {
                    mouse_event.event = MOUSE_MOVE;
                }
                else if (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    mouse_event.event = LEFT_CLICK;
                }
                else if (mouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
                {
                    mouse_event.event = RIGHT_CLICK;
                }
                else if (mouseEvent.dwEventFlags & MOUSE_WHEELED)
                {
                    if (mouseEvent.dwButtonState & 0xFF000000)
                        mouse_event.event = SCROLL_UP;
                    else
                        mouse_event.event = SCROLL_DOWN;
                }
                else if (mouseEvent.dwEventFlags & MOUSE_HWHEELED)
                {
                    // Handle horizontal scroll if necessary
                }
                else
                {
                    mouse_event.event = RELEASE;
                }
            }
        }

#else
        fd_set readfds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        for (auto &pair : key_states) pair.second = false;
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
                        // SGR mouse event detected
                        int button = 0, x = 0, y = 0;
                        char eventType = 0;
                        int j = i + 3;  // Start after "<"

                        // Parse button
                        while (j < nread && isdigit(buf[j]))
                        {
                            button = button * 10 + (buf[j] - '0');
                            j++;
                        }
                        if (j < nread && buf[j] == ';')
                            j++;  // Move past ';' if present

                        // Parse x coordinate
                        while (j < nread && isdigit(buf[j]))
                        {
                            x = x * 10 + (buf[j] - '0');
                            j++;
                        }
                        if (j < nread && buf[j] == ';')
                            j++;  // Move past ';' if present

                        // Parse y coordinate
                        while (j < nread && isdigit(buf[j]))
                        {
                            y = y * 10 + (buf[j] - '0');
                            j++;
                        }

                        // Get event type
                        if (j < nread)
                            eventType = buf[j];

                        // Check if it's a valid mouse event
                        if (eventType == 'M' || eventType == 'm')
                        {
                            mouse_pos = {x - 1, y - 1};   // SGR reports 1-based coordinates
                            mouse_event.x = (x - 1) / 2;  // Fix for double width characters
                            mouse_event.y = y - 1;
                            mouse_moved = true;  // Set mouse moved flag

                            // Determine the event type based on button
                            switch (button)
                            {
                                case 0:
                                    mouse_event.event = (eventType == 'M') ? Mouse_event_type::LEFT_CLICK : Mouse_event_type::LEFT_RELEASE;
                                    break;
                                case 1:
                                    mouse_event.event =
                                        (eventType == 'M') ? Mouse_event_type::MIDDLE_CLICK : Mouse_event_type::MIDDLE_RELEASE;
                                    break;
                                case 2:
                                    mouse_event.event =
                                        (eventType == 'M') ? Mouse_event_type::RIGHT_CLICK : Mouse_event_type::RIGHT_RELEASE;
                                    break;
                                case 64:
                                    mouse_event.event = (eventType == 'M') ? Mouse_event_type::SCROLL_UP : Mouse_event_type::MOUSE_MOVE;
                                    break;
                                case 65:
                                    mouse_event.event = (eventType == 'M') ? Mouse_event_type::SCROLL_DOWN : Mouse_event_type::MOUSE_MOVE;
                                    break;
                                default:
                                    mouse_event.event = Mouse_event_type::MOUSE_MOVE;
                                    break;
                            }

                            i = j - 1;  // Move i to the end of the parsed sequence
                        }
                    }
                    else if (buf[i] == '\033')
                    {  // Escape sequence for special keys
                        if (i + 1 < nread)
                        {
                            if (buf[i + 1] == '[')
                            {
                                // Handle arrow keys
                                // Handle arrow keys and function keys
                                if (i + 3 < nread)
                                {
                                    if (isdigit(buf[i + 2]))
                                    {
                                        int code = (buf[i + 2] - '0') * 10 + (buf[i + 3] - '0');
                                        switch (code)
                                        {
                                            case 11:
                                                key_states[KEY_F1] = true;
                                                break;
                                            case 12:
                                                key_states[KEY_F2] = true;
                                                break;
                                            case 13:
                                                key_states[KEY_F3] = true;
                                                break;
                                            case 14:
                                                key_states[KEY_F4] = true;
                                                break;
                                            case 15:
                                                key_states[KEY_F5] = true;
                                                break;
                                            case 16:
                                                key_states[KEY_F6] = true;
                                                break;
                                            case 17:
                                                key_states[KEY_F7] = true;
                                                break;
                                            case 18:
                                                key_states[KEY_F8] = true;
                                                break;
                                            case 19:
                                                key_states[KEY_F9] = true;
                                                break;
                                            case 20:
                                                key_states[KEY_F10] = true;
                                                break;
                                        }
                                        i += 3;  // Skip the sequence
                                    }
                                    else
                                    {
                                        // Handle arrow keys (existing code)
                                        switch (buf[i + 2])
                                        {
                                            case 'A':
                                                key_states[KEY_UP] = true;
                                                break;
                                            case 'B':
                                                key_states[KEY_DOWN] = true;
                                                break;
                                            case 'C':
                                                key_states[KEY_RIGHT] = true;
                                                break;
                                            case 'D':
                                                key_states[KEY_LEFT] = true;
                                                break;
                                        }
                                        i += 2;
                                    }
                                }
                            }
                        }
                    }
                    else if (buf[i] >= 1 && buf[i] <= 26)
                    {  // Control + [A-Z] or [a-z]
                        // Control key handling
                        Keys k = static_cast<Keys>(KEY_Ctrl_A + (buf[i] - 1));  // Ctrl + 'A' to 'Z'
                        Keys k_lower = static_cast<Keys>(KEY_Ctrl_a + (buf[i] - 1));
                        if (k >= KEY_Ctrl_A && k <= KEY_Ctrl_Z || k >= KEY_Ctrl_a && k <= KEY_Ctrl_z)
                        {
                            key_states[k_lower] = true;
                            key_states[k] = true;
                        }
                        Keys k2 = static_cast<Keys>(parse_key(buf[i]));
                        if (k2 != KEY_UNKNOWN)
                            key_states[k2] = true;
                    }
                    else
                    {
                        Keys k2 = static_cast<Keys>(parse_key(buf[i]));
                        if (k2 != KEY_UNKNOWN)
                            key_states[k2] = true;
                    }
                }
            }
        }
#endif
    }

    static void update_input_states() { update_mouse_and_key_states(); }
};

// TODO: Implement arrow keys too bruv...

// Initialize the static member
std::unordered_map<Keys, bool> Window::key_states;
utl::Vec<int, 2> Window::mouse_pos;
bool Window::mouse_moved;
Mouse_event Window::mouse_event;

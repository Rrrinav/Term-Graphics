#pragma once

#include <iostream>
#include <string>
#include "keys.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#endif

class Window
{
private:
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
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
#else
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
        system("clear");  // or "cls" on Windows
    }

    void cleanup_terminal()
    {
#ifdef _WIN32
        CloseHandle(hConsole);
#else
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
#endif
        system("clear");  // or "cls" on Windows
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
};

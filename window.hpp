// #ifndef RENDERER_HPP
// #define RENDERER_HPP
//
// #include <vector>
// #include <string>
// #include <iostream>
// #include <cstdlib>
//
// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <termios.h>
// #include <unistd.h>
// #endif
//
// class Renderer {
// private:
//     std::vector<std::vector<char>> buffer;
//     int width, height;
//
//     #ifdef _WIN32
//     HANDLE hConsole;
//     #else
//     struct termios orig_termios;
//     #endif
//
// public:
//     Renderer(int w, int h) : width(w), height(h) {
//         buffer.resize(height, std::vector<char>(width, ' '));
//         init_terminal();
//     }
//
//     ~Renderer() {
//         cleanup_terminal();
//     }
//
//     void init_terminal() {
//         #ifdef _WIN32
//         hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//         SetConsoleActiveScreenBuffer(hConsole);
//         #else
//         tcgetattr(STDIN_FILENO, &orig_termios);
//         struct termios raw = orig_termios;
//         raw.c_lflag &= ~(ECHO | ICANON);
//         tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
//         #endif
//         system("clear");  // or "cls" on Windows
//     }
//
//     void cleanup_terminal() {
//         #ifdef _WIN32
//         CloseHandle(hConsole);
//         #else
//         tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
//         #endif
//         system("clear");  // or "cls" on Windows
//     }
//
//     void draw_pixel(int x, int y, char c) {
//         if (x >= 0 && x < width && y >= 0 && y < height) {
//             buffer[y][x] = c;
//         }
//     }
//
//     void draw() {
//         std::string output;
//         for (const auto& row : buffer) {
//             output += std::string(row.begin(), row.end()) + "\n";
//         }
//
//         #ifdef _WIN32
//         DWORD written;
//         WriteConsoleOutputCharacter(hConsole, output.c_str(), output.length(), {0, 0}, &written);
//         #else
//         std::cout << "\033[H" << output << std::flush;
//         #endif
//     }
//
//     void clear() {
//         for (auto& row : buffer) {
//             std::fill(row.begin(), row.end(), ' ');
//         }
//     }
//
//     // ... (other methods like draw_line, draw_circle, etc.)
//
//     static Renderer create_buffer(int width, int height) {
//         return Renderer(width, height);
//     }
//
//     void sleep(int ms) {
//         #ifdef _WIN32
//         Sleep(ms);
//         #else
//         usleep(ms * 1000);
//         #endif
//     }
// };
//
// #endif // RENDERER_HPP

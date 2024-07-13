#include <iostream>
#include <locale>
#include <string>
#include <vector>

int main()
{
    // Set the locale to the user's environment default
    std::locale::global(std::locale(""));

    // Define a gradient using a series of Unicode characters
    std::vector<wchar_t> gradient = {L' ', L'░', L'▒', L'▓', L'█'};

    // Define the corresponding colors for each gradient character
    std::vector<std::wstring> colors = {
        L"\033[38;2;255;255;255m",  // White
        L"\033[38;2;192;192;192m",  // Light gray
        L"\033[38;2;128;128;128m",  // Gray
        L"\033[38;2;64;64;64m",     // Dark gray
        L"\033[38;2;0;0;0m"         // Black
    };

    // Number of repetitions for each character
    const int repetitions = 10;

    // Display the gradient with colors
    for (size_t i = 0; i < gradient.size(); ++i)
    {
        for (int j = 0; j < colors.size(); ++j)
        {
            std::wcout << colors[j];
            std::wcout << gradient[i] << gradient[i];
        }
        std::wcout << std::endl;

    }

    // Reset color
    std::wcout << L"\033[0m" << std::endl;

    return 0;
}

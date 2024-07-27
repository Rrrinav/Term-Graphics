#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

class Time
{
public:
    // Get current time in seconds since epoch
    static std::time_t now() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); }

    // Get current time in milliseconds since epoch
    static std::chrono::milliseconds now_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }

    // Get current date as string (format: YYYY-MM-DD)
    static std::string current_date()
    {
        auto t = now();
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }

    // Get current time as string (format: HH:MM:SS)
    static std::string current_time()
    {
        auto t = now();
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    // Get current date and time as string (format: YYYY-MM-DD HH:MM:SS)
    static std::string current_date_time()
    {
        auto t = now();
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

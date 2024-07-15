#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#define L_GEBRA_IMPLEMENTATION
#include "../l_gebra/l_gebra.hpp"

class Glyph
{
private:
    std::vector<std::string> data;
    int width = 0;
    int height = 0;

public:
    Glyph() = default;
    Glyph(const std::string &filename) { load_from_file(filename); }
    Glyph(int width, int height, char fill = ' ')
    {
        data = std::vector<std::string>(height, std::string(width, fill));
        this->width = width;
        this->height = height;
    }
    Glyph(std::vector<std::string> data) : data(data)
    {
        height = data.size();
        width = 0;
        for (const auto &line : data)
            if (line.length() > width)
                width = line.length();
        pad_lines_to_width();
    }
    Glyph(const Glyph &other) : data(other.data) {}
    ~Glyph() = default;
    char get_char(int x, int y) const
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return ' ';  // Return space if out of bounds
        return data[y][x];
    }
    int get_width() const { return width; }
    int get_height() const { return height; }
    std::vector<std::string> get_data() const { return data; }
    bool load_from_file(const std::string &filename)
    {
        width = 0;
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open sprite file: " + filename << std::endl;
            return false;
        }

        data.clear();
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;  // Skip empty lines
            data.push_back(line);
            if (line.length() > width)
                width = line.length();  // Update width to the maximum line length
        }
        height = data.size();  // Update height to the number of lines
        file.close();

        pad_lines_to_width();  // Ensure all lines have the same width
        return true;
    }

    void pad_lines_to_width()
    {
        for (auto &line : data)
            if (line.length() < width)
                line.append(width - line.length(), ' ');  // Pad with spaces
    }

    Glyph rotate(float angle, utl::Vec<int, 2> start_pos) const
    {
        int width = get_width();
        int height = get_height();

        // Convert angle to radians
        float cos_theta = std::cos(angle);
        float sin_theta = std::sin(angle);

        // Calculate the center of rotation based on the start position
        float cx = start_pos[0] + width / 2.0f;
        float cy = start_pos[1] + height / 2.0f;

        // Keep the same dimensions
        int new_width = width;
        int new_height = height;

        std::vector<std::string> new_data(new_height, std::string(new_width, ' '));

        // Rotate
        for (int y = 0; y < new_height; ++y)
        {
            for (int x = 0; x < new_width; ++x)
            {
                // Calculate position relative to the center of rotation
                float dx = (start_pos[0] + x) - cx;
                float dy = (start_pos[1] + y) - cy;

                // Rotate
                int src_x = static_cast<int>(std::round((dx * cos_theta - dy * sin_theta) + cx)) - start_pos[0];
                int src_y = static_cast<int>(std::round((dx * sin_theta + dy * cos_theta) + cy)) - start_pos[1];

                // Copy pixel if it's within the original sprite bounds
                if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height)
                    new_data[y][x] = data[src_y][src_x];
            }
        }

        return Glyph(new_data);
    }

    utl::Vec<int, 4> get_bounds() const
    {
        int min_x = width;
        int max_x = 0;
        int min_y = height;
        int max_y = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (data[y][x] != ' ')
                {
                    min_x = std::min(min_x, x);
                    max_x = std::max(max_x, x);
                    min_y = std::min(min_y, y);
                    max_y = std::max(max_y, y);
                }
            }
        }
        return {min_x, min_y, max_x, max_y};
    }

    Glyph scale(float scale_x, float scale_y)
    {
        int new_width = static_cast<int>(width * scale_x);
        int new_height = static_cast<int>(height * scale_y);
        std::vector<std::string> new_data(new_height, std::string(new_width, ' '));
        auto data = get_data();
        for (int y = 0; y < new_height; y++)
        {
            for (int x = 0; x < new_width; x++)
            {
                int old_x = static_cast<int>(x / scale_x);
                int old_y = static_cast<int>(y / scale_y);
                new_data[y][x] = data[old_y][old_x];
            }
        }

        return Glyph(new_data);
    }
};

class Animated_sprite
{
    std::vector<Glyph> frames;
    int current_frame;
    float frame_time;
    float frame_duration;
    float scale_x;
    float scale_y;
    int FPS;

public:
    Animated_sprite() : current_frame(0), frame_time(0), frame_duration(0), scale_x(1.0f), scale_y(1.0f), FPS(1) {}
    Animated_sprite(const std::vector<Glyph> &frames, float frame_duration, float scale_x = 1.0f, float scale_y = 1.0f, int FPS = 1)
        : frames(frames), current_frame(0), frame_time(0), frame_duration(frame_duration), scale_x(scale_x), scale_y(scale_y), FPS(FPS)
    {
    }

    void load_from_file(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open animated sprite file: " + filename << std::endl;
            return;
        }

        std::string line;
        int frame_number = 0;
        std::vector<std::string> current_frame_data;

        while (std::getline(file, line))
        {
            int num_digits_in_frame_number = 0;
            int frame_num = frame_number;
            bool is_frame_number = false;
            while (frame_num > 0)
            {
                frame_num /= 10;
                num_digits_in_frame_number++;
            }
            for (size_t i = 0; i < line.size(); i++)
            {
                if (std::isdigit(line[i]) && i <= num_digits_in_frame_number)
                {
                    is_frame_number = true;
                }
                else
                {
                    is_frame_number = false;
                    break;
                }
            }

            if (line[0] == '#')  // Skip comments
                continue;

            if (line.empty())  // End of current frame
            {
                if (!current_frame_data.empty())
                {
                    frames.emplace_back(current_frame_data);
                    current_frame_data.clear();
                }
            }
            else if (is_frame_number)  // Frame number
            {
                if (!current_frame_data.empty())
                {
                    frames.emplace_back(current_frame_data);
                    current_frame_data.clear();
                }
                frame_number = std::stoi(line);
            }
            else  // Frame data
            {
                // Ensure the width consistency by padding with spaces
                if (!current_frame_data.empty())
                {
                    size_t max_width = 0;
                    for (const auto &row : current_frame_data) max_width = std::max(max_width, row.size());
                    for (auto &row : current_frame_data) row.resize(max_width, ' ');
                }
                current_frame_data.push_back(line);
            }
        }

        // Add the last frame if any
        if (!current_frame_data.empty())
            frames.emplace_back(current_frame_data);

        file.close();
    }

    Glyph get_current_frame() const { return frames[current_frame]; }
    Glyph get_frame(int index) const { return frames[index]; }
    Glyph update(float dt)
    {
        frame_time += dt;
        if (frame_time >= frame_duration)
        {
            frame_time = 0;
            current_frame = (current_frame + 1) % frames.size();
        }
        return frames[current_frame];
    }
    size_t get_frame_count() const { return frames.size(); }
    void set_frame_duration(float duration) { frame_duration = duration; }
};

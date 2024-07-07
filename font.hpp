#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Glyph
{
public:
    std::vector<std::string> lines;
    int width;
    int height;

    // Default constructor
    Glyph() : lines(), width(0), height(0) {}

    Glyph(const std::vector<std::string> &_lines) : lines(_lines)
    {
        height = lines.size();
        width = lines.empty() ? 0 : lines[0].length();
    }

    Glyph(const Glyph &other) : lines(other.lines), width(other.width), height(other.height) {}

    int get_width() const { return width; }
    int get_height() const { return height; }
    const std::vector<std::string> &get_lines() const { return lines; }
    std::string get_line(int i) const { return lines[i]; }
    char get_char(int x, int y) const { return lines[y][x]; }
    bool is_empty() const { return lines.empty(); }
    std::vector<std::string> get_lines() { return lines; }
};

class Font
{
private:
    std::unordered_map<char, Glyph> glyphs;

public:
    Font() : glyphs(std::unordered_map<char, Glyph>()) {}
    Font(const std::string &filename) { load_from_file(filename); }
    Font(std::unordered_map<char, Glyph> _glyphs) : glyphs(_glyphs) {}
    Font(std::unordered_map<char, Glyph> &_glyphs) : glyphs(_glyphs) {}
    Font(std::unordered_map<char, std::vector<std::string>> _glyphs)
    {
        for (auto &[c, lines] : _glyphs)
        {
            glyphs[c] = Glyph(lines);
        }
    }
    Font(std::vector<std::pair<char, std::vector<std::string>>> _glyphs)
    {
        for (auto &[c, lines] : _glyphs)
        {
            glyphs[c] = Glyph(lines);
        }
    }
    Font(const Font &other) : glyphs(other.glyphs) {}
    Font(Font &&other) : glyphs(std::move(other.glyphs)) {}
    bool load_from_file(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open font file: " + filename << std::endl;
            return false;
        }

        char current_char;
        std::vector<std::string> current_glyph;
        std::string line;

        while (file >> current_char)
        {
            file.ignore();  // Skip the space after the character

            current_glyph.clear();
            while (std::getline(file, line) && !line.empty())
            {
                current_glyph.push_back(line);
            }

            glyphs[current_char] = Glyph(current_glyph);
        }
        return true;
    }

    const Glyph &get_glyph(char c) const
    {
        static const Glyph default_glyph({" "});  // Default glyph representation
        auto it = glyphs.find(c);
        if (it != glyphs.end())
        {
            return it->second;
        }
        // Print a warning message
        std::cerr << "Warning: Glyph for character '" << c << "' not found. Using default glyph." << std::endl;
        return default_glyph;
    }

    Font &operator=(const Font &other)
    {
        if (this != &other)
        {
            glyphs = other.glyphs;
        }
        return *this;
    }

    Font &operator=(Font &&other) noexcept
    {
        if (this != &other)
        {
            glyphs = std::move(other.glyphs);
        }
        return *this;
    }

    bool has_glyph(char c) const { return glyphs.find(c) != glyphs.end(); }

    void add_glyph(char c, const std::vector<std::string> &lines) { glyphs[c] = Glyph(lines); }

    std::vector<char> get_available_chars() const
    {
        std::vector<char> chars;
        chars.reserve(glyphs.size());
        for (const auto &[c, _] : glyphs)
        {
            chars.push_back(c);
        }
        return chars;
    }
};

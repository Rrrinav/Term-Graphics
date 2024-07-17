#pragma once

// Totally(emphasis!) inspired by OneLoneCoder's code : https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_PerlinNoise.cpp
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <iostream>

class Perlin_noise
{
    size_t _size;
    size_t _2d_width;
    size_t _2d_height;
    std::vector<float> _seed_1D;
    std::vector<float> _seed_2D;

public:
    Perlin_noise() = default;
    Perlin_noise(size_t size, size_t width2D, size_t height2D) : _size(size), _2d_height(height2D), _2d_width(width2D)
    {
        _seed_1D.resize(size);
        for (size_t i = 0; i < size; i++) _seed_1D[i] = (float)rand() / (float)RAND_MAX;
        _seed_2D.resize(width2D * height2D);
        for (size_t i = 0; i < width2D * height2D; i++) _seed_2D[i] = (float)rand() / (float)RAND_MAX;
    }

    std::vector<float> get1D_noise(size_t octaves, float bias = 0.5f)
    {
        size_t count = _size;
        if (count == 0)
            return std::vector<float>();

        if (_size >> octaves < 1)
            std::cerr << "Octaves is too high for the size of the noise\n";

        std::vector<float> result;
        result.resize(count, 0.0f);
        for (size_t i = 0; i < count; i++)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scale_acc = 0.0f;
            for (size_t o = 0; o < octaves; o++)
            {
                size_t pitch = _size >> o;
                size_t sample_1 = (i / pitch) * pitch;
                size_t sample_2 = (sample_1 + pitch) % _size;
                float blend = (float)(i - sample_1) / (float)pitch;
                float sample = (1.0f - blend) * _seed_1D[sample_1] + blend * _seed_1D[sample_2];
                noise += sample * scale;
                scale_acc += scale;
                scale /= bias;
            }
            result[i] = noise / scale_acc;
        }
        return result;
    }

    std::vector<float> get2D_noise(size_t octaves, float bias = 0.5f)
    {
        size_t count = _2d_width * _2d_height;
        if (count == 0)
            return std::vector<float>();
        if (_2d_width >> octaves < 1 || _2d_height >> octaves < 1)
            std::cerr << "Octaves is too high for the size of the noise\n";
        std::vector<float> result;
        result.resize(count, 0.0f);
        for (size_t x = 0; x < _2d_width; x++)
            for (size_t y = 0; y < _2d_height; y++)
            {
                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;
                for (size_t o = 0; o < octaves; o++)
                {
                    size_t pitch_x = _2d_width >> o;
                    size_t pitch_y = _2d_height >> o;
                    size_t sample_x1 = (x / pitch_x) * pitch_x;
                    size_t sample_x2 = (sample_x1 + pitch_x) % _2d_width;
                    size_t sample_y1 = (y / pitch_y) * pitch_y;
                    size_t sample_y2 = (sample_y1 + pitch_y) % _2d_height;
                    float blend_x = (float)(x - sample_x1) / (float)pitch_x;
                    float blend_y = (float)(y - sample_y1) / (float)pitch_y;
                    float sample_1 = (1.0f - blend_x) * _seed_2D[sample_x1 + sample_y1 * _2d_width] +
                                     blend_x * _seed_2D[sample_x2 + sample_y1 * _2d_width];
                    float sample_2 = (1.0f - blend_x) * _seed_2D[sample_x1 + sample_y2 * _2d_width] +
                                     blend_x * _seed_2D[sample_x2 + sample_y2 * _2d_width];
                    noise += (1.0f - blend_y) * sample_1 + blend_y * sample_2;
                    scale_acc += scale;
                    scale /= bias;
                }
                result[y * _2d_width + x] = noise / scale_acc;
            }
        return result;
    }
    void change_seed1D()
    {
        for (size_t i = 0; i < _size; i++) _seed_1D[i] = (float)rand() / (float)RAND_MAX;
    }

    void change_seed2D()
    {
        for (size_t i = 0; i < _2d_width * _2d_height; i++) _seed_2D[i] = (float)rand() / (float)RAND_MAX;
    }
};

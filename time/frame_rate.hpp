#pragma once

#include <chrono>
#include <thread>

class Frame_rate
{
private:
    int target_fps;
    int actual_fps;
    int frame_count;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_frame_time;
    std::chrono::milliseconds target_frame_duration;

public:
    Frame_rate()
        : target_fps(24),
          actual_fps(0),
          frame_count(0),
          start_time(std::chrono::high_resolution_clock::now()),
          last_frame_time(std::chrono::high_resolution_clock::now()),
          target_frame_duration(1000 / target_fps)
    {
    }

    Frame_rate(int target_fps)
        : target_fps(target_fps),
          actual_fps(0),
          frame_count(0),
          start_time(std::chrono::high_resolution_clock::now()),
          last_frame_time(std::chrono::high_resolution_clock::now()),
          target_frame_duration(1000 / target_fps)
    {
    }

    void set_target_fps(int fps) { target_fps = fps; }

    int get_target_fps() const { return target_fps; }

    int get_actual_fps() const { return actual_fps; }

    // Function to get elapsed time since the start of the program
    double get_elapsed_time() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() /
               1000.0;
    }

    // Function to get delta time between frames
    double get_delta_time() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_frame_time).count() /
               1000.0;
    }

    // Function to start a new frame
    void start_frame() { last_frame_time = std::chrono::high_resolution_clock::now(); }

    void end_frame() { syn_fps(); }

    void reset()
    {
        start_time = std::chrono::high_resolution_clock::now();
        last_frame_time = std::chrono::high_resolution_clock::now();
        actual_fps = 0;
        frame_count = 0;
    }

    void reset_frame_count() { frame_count = 0; }

    void reset_frame_time() { last_frame_time = std::chrono::high_resolution_clock::now(); }

    void adjust_target_fps(int adjustment)
    {
        target_fps += adjustment;
        if (target_fps < 1)
            target_fps = 1;
    }

    int get_frame_count() const { return frame_count; }

    // Function to end the current frame and adjust the timing to match the target FPS
    void syn_fps()
    {
        frame_count++;
        auto now = std::chrono::high_resolution_clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame_time);

        // Calculate the actual FPS over one second
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() >= 1)
        {
            actual_fps = frame_count;
            frame_count = 0;
            start_time = now;
        }

        // Adjust the frame timing to match the target FPS
        if (frame_duration < target_frame_duration)
            std::this_thread::sleep_for(target_frame_duration - frame_duration);
    }

    // Additional functions for convenience
    void pause(int milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }  // Simple pause method
};

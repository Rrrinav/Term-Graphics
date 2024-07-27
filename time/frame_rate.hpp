#pragma once
#include <chrono>
#include <thread>

class Frame_rate
{
  private:
  int target_fps;
  int actual_fps;
  int frame_count;
  std::chrono::time_point<std::chrono::high_resolution_clock> program_start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> fps_start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_time;
  std::chrono::duration<double> target_frame_duration;
  std::chrono::time_point<std::chrono::high_resolution_clock> last_interval_check;
  double last_frame_duration;

  public:
  Frame_rate()
      : target_fps(24),
        actual_fps(0),
        frame_count(0),
        program_start_time(std::chrono::high_resolution_clock::now()),
        fps_start_time(std::chrono::high_resolution_clock::now()),
        frame_start_time(std::chrono::high_resolution_clock::now()),
        target_frame_duration(1.0 / target_fps),
        last_interval_check(std::chrono::high_resolution_clock::now()),
        last_frame_duration(0.0)
  {
  }

  Frame_rate(int target_fps)
      : target_fps(target_fps),
        actual_fps(0),
        frame_count(0),
        program_start_time(std::chrono::high_resolution_clock::now()),
        fps_start_time(std::chrono::high_resolution_clock::now()),
        frame_start_time(std::chrono::high_resolution_clock::now()),
        target_frame_duration(1.0 / target_fps),
        last_interval_check(std::chrono::high_resolution_clock::now()),
        last_frame_duration(0.0)
  {
  }

  void set_target_fps(int fps)
  {
    target_fps = fps;
    target_frame_duration = std::chrono::duration<double>(1.0 / target_fps);
  }

  int get_target_fps() const { return target_fps; }
  int get_actual_fps() const { return actual_fps; }

  double get_elapsed_time() const
  {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - program_start_time).count();
  }

  double get_delta_time() const { return last_frame_duration; }

  void start_frame() { frame_start_time = std::chrono::high_resolution_clock::now(); }

  void end_frame()
  {
    auto now = std::chrono::high_resolution_clock::now();
    last_frame_duration = std::chrono::duration<double>(now - frame_start_time).count();
    frame_count++;

    auto elapsed = std::chrono::duration<double>(now - fps_start_time).count();

    if (elapsed >= 1.0)
    {
      actual_fps = static_cast<int>(frame_count / elapsed);
      frame_count = 0;
      fps_start_time = now;
    }

    auto frame_end = frame_start_time + target_frame_duration;
    if (now < frame_end)
      std::this_thread::sleep_until(frame_end);
  }

  void reset()
  {
    program_start_time = std::chrono::high_resolution_clock::now();
    fps_start_time = std::chrono::high_resolution_clock::now();
    frame_start_time = std::chrono::high_resolution_clock::now();
    last_interval_check = std::chrono::high_resolution_clock::now();
    actual_fps = 0;
    frame_count = 0;
    last_frame_duration = 0.0;
  }

  void reset_frame_count() { frame_count = 0; }

  void adjust_target_fps(int adjustment)
  {
    target_fps += adjustment;
    if (target_fps < 1)
      target_fps = 1;
    target_frame_duration = std::chrono::duration<double>(1.0 / target_fps);
  }

  int get_frame_count() const { return frame_count; }

  void pause(int milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }

  bool has_passed(float seconds)
  {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration<double>(now - last_interval_check).count();

    if (elapsed >= seconds)
    {
      last_interval_check = now;
      return true;
    }
    return false;
  }
};

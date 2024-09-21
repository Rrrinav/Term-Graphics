#include <vector>
#define RENDERER_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include "../dependencies/image.hpp"
#include "../renderer2D/ascii.hpp"

// You will need to have frames of the video in the assets/Frames directory
// I removed the frames from the repository because they are too large
// Use ffmpeg to extract frames from the video
// I will think of a way to do it automatically in the future

std::vector<Sprite> load_video_frames(const std::string &frame_path_pattern, int total_frames)
{
  std::vector<Sprite> frames;
  frames.reserve(total_frames);
  std::cout << "Loading " << total_frames << " frames..." << std::endl;
  for (int i = 1; i <= total_frames; ++i)
  {
    std::ostringstream frame_path;
    frame_path << frame_path_pattern << std::setfill('0') << std::setw(4) << i << ".png";

    utl::Image image(frame_path.str());               // Assuming utl::Image can load images
    Sprite sprite = image.convert_image_to_sprite();  // Assuming Sprite is a class that can hold image data
    frames.emplace_back(sprite);
  }
  std::cout << "Loaded! " << std::endl;
  return frames;
}

void play_video(const std::vector<Sprite> &sprites, Renderer &renderer, int fps)
{
  int delay = 1000 / fps;  // Delay between frames in milliseconds

  for (const auto &sprite : sprites)
  {
    renderer.empty();                                               // Clear the screen or buffer
    renderer.reset_screen();                                        // Set the cursor to the top left corner
    renderer.draw_sprite({0, 0}, sprite);                           // Draw the sprite at the desired position
    renderer.print();                                               // Display the frame
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));  // Wait for the next frame
  }
}

int main()
{
  auto frames = load_video_frames("../assets/Frames/frame_", 999);
  size_t width = frames[0].width();
  size_t height = frames[0].height();
  Renderer r(width / 2, height);
  play_video(frames, r, 24);
  std::cin.get();
  return 0;
}

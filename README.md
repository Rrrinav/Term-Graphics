# ASCII Graphics Engine

A powerful and flexible ASCII graphics rendering engine with advanced features for creating stunning console-based visuals and interactive applications.

## Features

- **High-performance rendering**: Optimized for speed and efficiency
- **Rich shape drawing**: Lines, circles, rectangles, triangles, polygons, and more
- **Anti-aliasing**: Smooth line and shape rendering for improved visual quality
- **Color support**: Full RGB color capabilities with transparency
- **Text rendering**: Custom font support with shadowing effects
- **Sprite system**: Load and render sprite animations
- **Input handling**: Keyboard and mouse input support
- **Window management**: Easy-to-use window controls
- **Caching system**: Improved performance for repetitive rendering tasks

## Quick Start

```cpp
#define RENDERER_IMPLEMENTATION
#include "renderer.hpp"

int main() {
    auto buffer = Renderer::create_buffer(100, 80);
    Renderer renderer(buffer);

    renderer.Init();

    while (true) {
        renderer.empty();
        renderer.reset_screen();

        // Draw your graphics here
        renderer.draw_circle({50, 40}, 20, 'o', GREEN);
        renderer.draw_text({10, 10}, "Hello, ASCII World!", BRIGHT_WHITE);

        renderer.draw();
        renderer.sleep(1000 / 60); // 60 FPS

        if (Window::is_pressed(Keys::KEY_ESC)) break;
    }

    return 0;
}
```

## Advanced Usage

### Sprite animation

```cpp
sprite.load_from_file("animation.txt");
renderer.draw_sprite({10, 10}, animated_sprite.get_frame(frame));
```

### Shapes and anti-aliasing

```cpp
renderer.draw_anti_aliased_line({10, 10}, {90, 70}, '-', BLUE);
renderer.draw_fill_antialias_triangleInstallation
```

### Sprites animations

```cpp
Animated_sprite animated_sprite;
animated_sprite.load_from_file("animation.txt");
renderer.draw_sprite({10, 10}, animated_sprite.get_frame(frame));
```

### User input and interaction

```cpp
if (Window::is_pressed(Keys::KEY_SPACE)) {
    // Perform action on spacebar press
}

auto mouse_pos = Window::get_mouse_pos();
renderer.draw_point(mouse_pos, 'x', YELLOW);
```

## Installation

Clone the repository
This is a header only library, so you can simply include the header files in your project.
Compile with your preferred C++ compiler (C++11 or later required)

## Contributing

All contributions are welcome and appreciated. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT License](https://choosealicense.com/licenses/mit/)

## Acknowledgements

Rinav ( Rrrinav ) (original author)

### STILL IN DEVELOPMENT

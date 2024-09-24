#define RENDERER_IMPLEMENTATION
#include "../renderer2D/ascii.hpp"
#include "../time/frame_rate.hpp"

class Particle
{
private:
  utl::Vec<float, 2> _position;
  utl::Vec<float, 2> _velocity;
  utl::Vec<float, 2> _acceleration;
  float _mass;
  float _radius;
  Color _color;
  float _lifetime;
  float _age;

public:
  Particle() = default;

  Particle(utl::Vec<float, 2> pos, utl::Vec<float, 2> vel, utl::Vec<float, 2> acc, float m, float r, Color c, float lifetime)
      : _position(pos), _velocity(vel), _acceleration(acc), _mass(m), _radius(r), _color(c), _lifetime(lifetime), _age(0)
  {
  }

  void update(float deltaTime, const utl::Vec<float, 2> &gravity, float dragCoefficient)
  {
    // Update age
    _age += deltaTime;
    if (_age >= _lifetime)
      return;

    // Update velocity and position
    _velocity += (_acceleration + gravity) * deltaTime;
    _velocity *= (1.0f - dragCoefficient);  // Apply drag
    _position += _velocity * deltaTime;
  }

  bool is_alive() const { return _age < _lifetime; }

  utl::Vec<float, 2> position() const { return _position; }
  char symbol() const { return '*'; }  // Assuming '*' as a default symbol for particles
  Color color() const { return _color; }
};

class ParticleSystem
{
private:
  std::vector<Particle> particles;

public:
  void add_particle(const Particle &particle) { particles.push_back(particle); }

  void update(float deltaTime, const utl::Vec<float, 2> &gravity, float dragCoefficient)
  {
    for (auto &particle : particles) particle.update(deltaTime, gravity, dragCoefficient);

    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle &p) { return !p.is_alive(); }), particles.end());
  }

  void render(Renderer &renderer)
  {
    for (const auto &particle : particles)
    {
      if (particle.is_alive())
      {
        renderer.draw_point(
            {static_cast<int>(particle.position().x()), static_cast<int>(particle.position().y())}, particle.symbol(), particle.color());
      }
    }
  }

  void create_explosion(utl::Vec<float, 2> position, int particle_count, float speed, float minLifespan, float maxLifespan, Color minColor,
                        Color maxColor)
  {
    for (int i = 0; i < particle_count; ++i)
    {
      float angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;                                            // Random direction
      float particleSpeed = speed * (1.0f + static_cast<float>(rand()) / RAND_MAX * 1.5f);                       // Random speed
      utl::Vec<float, 2> velocity = {particleSpeed * std::cos(angle), particleSpeed * std::sin(angle) + speed};  // Initial upward force

      float lifespan = minLifespan + static_cast<float>(rand()) / RAND_MAX * (maxLifespan - minLifespan);  // Random lifespan
      Color color = minColor.blend(maxColor, static_cast<float>(rand()) / RAND_MAX);                       // Random color

      add_particle(Particle(position, velocity, {0, 0}, 1.0f, 1.0f, color, lifespan));
    }
  }
};

int main()
{
  Renderer renderer(150, 80);
  renderer.set_bg_color(utl::Color_codes::GRAY_3);
  utl::Vec<float, 2> gravity = {0.0f, 9.8f};  // Gravity vector (downward)
  float dragCoefficient = 0.01f;
  ParticleSystem particleSystem;
  Frame_rate frame(60);
  while (true)
  {
    frame.start_frame();
    Window::update_input_states();
    renderer.empty();
    renderer.clear_screen();
    renderer.draw_text({0, 0}, "Left click to create an explosion", utl::Color_codes::WHITE);
    // Create an explosion at the center of the screen every second for demonstration
    static float lastExplosionTime = 0.0f;
    float currentTime = static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
    auto dt = frame.get_delta_time();

    if (Window::has_mouse_moved())
    {
      auto mouse = Window::get_mouse_event();
      lastExplosionTime = currentTime;
      if (mouse.event == Mouse_event_type::LEFT_CLICK)
        particleSystem.create_explosion({(float)mouse.x, (float)mouse.y},
                                        100,
                                        10.0f,
                                        2.0f,
                                        5.0f,
                                        utl::Color_codes::RED,
                                        utl::Color_codes::BLUE);  // Adjust parameters as needed
    }
    // Update and render particles
    particleSystem.update(dt, gravity, dragCoefficient);
    particleSystem.render(renderer);

    renderer.print();
    renderer.sleep(1000 / 60);  // 60 FPS
    frame.end_frame();
  }

  return 0;
}

#pragma once
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource {
class ResourceManager;
}  // namespace engine::resource

namespace engine::render {
class Camera;
class Renderer;
}  // namespace engine::render

namespace engine::core {
class Time;
class GameApp {
 public:
  GameApp();

  ~GameApp();

  void Run();

  GameApp(const GameApp&) = delete;

  GameApp& operator=(const GameApp&) = delete;

  GameApp(GameApp&&) = delete;

  GameApp& operator=(GameApp&&) = delete;

 private:
  [[nodiscard]] bool Init();

  void Update(double delta_time_s);

  void Render();

  void HandleEvents();

  void Close();

  [[nodiscard]] bool InitSDL();
  [[nodiscard]] bool InitResourceManager();
  [[nodiscard]] bool InitTime();
  [[nodiscard]] bool InitRenderer();
  [[nodiscard]] bool InitCamera();

 private:
  SDL_Window* sdl_window_{nullptr};
  SDL_Renderer* sdl_renderer_{nullptr};
  bool is_running_{true};
  std::unique_ptr<Time> time_{nullptr};
  std::unique_ptr<engine::resource::ResourceManager> resource_manager_{nullptr};
  std::unique_ptr<engine::render::Camera> camera_{nullptr};
  std::unique_ptr<engine::render::Renderer> renderer_{nullptr};
};
}  // namespace engine::core

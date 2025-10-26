#pragma once
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

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

 private:
  SDL_Window* window_{nullptr};
  SDL_Renderer* renderer_{nullptr};
  bool is_running_{true};
  std::unique_ptr<Time> time_{nullptr};
};
}  // namespace engine::core

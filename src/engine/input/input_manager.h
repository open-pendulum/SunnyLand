#pragma once
#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace engine::core {
class Config;
}  // namespace engine::core

namespace engine::input {

enum class ActionState {
  kInactive,
  kPressedThisFrame,
  kHeldDown,
  kReleasedThisFrame
};

class InputManager final {
 public:
  explicit InputManager(SDL_Renderer* sdl_renderer,
                        const engine::core::Config* config);

  void Update();

  bool IsActionDown(std::string_view action_name) const;
  bool IsActionPressed(std::string_view action_name) const;
  bool IsActionReleased(std::string_view action_name) const;

  bool ShouldQuit() const;
  void SetShouldQuit(bool should_quit);

  glm::vec2 GetMousePosition() const;
  glm::vec2 GetLogicalMousePosition() const;

 private:
  void ProcessEvent(const SDL_Event& event);
  void InitializeMappings(const engine::core::Config* config);

  void UpdateActionState(std::string_view action_name, bool is_input_active,
                         bool is_repeat_event);
  SDL_Scancode ScancodeFromString(std::string_view key_name);
  Uint32 MouseButtonFromString(std::string_view button_name);

 private:
  SDL_Renderer* sdl_renderer_;
  std::unordered_map<std::string, std::vector<std::string>>
      actions_to_keyname_map_;
  std::unordered_map<std::variant<SDL_Scancode, Uint32>,
                     std::vector<std::string>>
      input_to_actions_map_;

  std::unordered_map<std::string, ActionState> action_states_;

  bool should_quit_ = false;
  glm::vec2 mouse_position_;
};

}  // namespace engine::input
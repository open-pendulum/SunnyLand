#include "input_manager.h"
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <logger.hpp>
#include <stdexcept>
#include "core/config.h"

namespace engine::input {
namespace {
DECLARE_TAG(InputManager)
}  // namespace

InputManager::InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config)
    : sdl_renderer_(sdl_renderer) {
  if (!sdl_renderer_) {
    LOGE(TAG, "SDL_Renderer is nullptr");
    throw std::runtime_error("SDL_Renderer is nullptr");
  }
  InitializeMappings(config);
  float x, y;
  SDL_GetMouseState(&x, &y);
  mouse_position_ = {x, y};
  LOGI(TAG, "Init mouse position: ({}, {})", mouse_position_.x, mouse_position_.y);
}

void InputManager::Update() {
  for (auto& state : action_states_ | std::views::values) {
    if (state == ActionState::kPressedThisFrame) {
      state = ActionState::kHeldDown;
    } else if (state == ActionState::kReleasedThisFrame) {
      state = ActionState::kInactive;
    }
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ProcessEvent(event);
  }
}

void InputManager::ProcessEvent(const SDL_Event& event) {
  switch (event.type) {
  case SDL_EVENT_KEY_DOWN:
  case SDL_EVENT_KEY_UP: {
    SDL_Scancode scancode = event.key.scancode;
    bool is_down = event.key.down;
    bool is_repeat = event.key.repeat;

    auto it = input_to_actions_map_.find(scancode);
    if (it != input_to_actions_map_.end()) {
      const std::vector<std::string>& associated_actions = it->second;
      for (const auto& action_name : associated_actions) {
        UpdateActionState(action_name, is_down, is_repeat);
      }
    }
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  case SDL_EVENT_MOUSE_BUTTON_UP: {
    const Uint32 button = event.button.button;
    bool is_down = event.button.down;
    const auto it = input_to_actions_map_.find(button);
    if (it != input_to_actions_map_.end()) {
      const std::vector<std::string>& associated_actions = it->second;
      for (const auto& action_name : associated_actions) {
        UpdateActionState(action_name, is_down, false);
      }
    }

    mouse_position_ = {event.button.x, event.button.y};
    break;
  }
  case SDL_EVENT_MOUSE_MOTION:
    mouse_position_ = {event.motion.x, event.motion.y};
    break;
  case SDL_EVENT_QUIT:
    should_quit_ = true;
    break;
  default:
    break;
  }
}

bool InputManager::IsActionDown(std::string_view action_name) const {
  if (const auto it = action_states_.find(std::string(action_name)); it != action_states_.end()) {
    return it->second == ActionState::kPressedThisFrame || it->second == ActionState::kHeldDown;
  }
  return false;
}

bool InputManager::IsActionPressed(std::string_view action_name) const {
  if (const auto it = action_states_.find(std::string(action_name)); it != action_states_.end()) {
    return it->second == ActionState::kPressedThisFrame;
  }
  return false;
}

bool InputManager::IsActionReleased(std::string_view action_name) const {
  if (const auto it = action_states_.find(std::string(action_name)); it != action_states_.end()) {
    return it->second == ActionState::kReleasedThisFrame;
  }
  return false;
}

bool InputManager::ShouldQuit() const {
  return should_quit_;
}

void InputManager::SetShouldQuit(bool should_quit) {
  should_quit_ = should_quit;
}

glm::vec2 InputManager::GetMousePosition() const {
  return mouse_position_;
}

glm::vec2 InputManager::GetLogicalMousePosition() const {
  glm::vec2 logical_pos;
  SDL_RenderCoordinatesFromWindow(sdl_renderer_, mouse_position_.x, mouse_position_.y, &logical_pos.x, &logical_pos.y);
  return logical_pos;
}

void InputManager::InitializeMappings(const engine::core::Config* config) {
  spdlog::trace("初始化输入映射...");
  if (!config) {
    spdlog::error("输入管理器: Config 为空指针");
    throw std::runtime_error("输入管理器: Config 为空指针");
  }
  actions_to_keyname_map_ = config->InputMappings();
  input_to_actions_map_.clear();
  action_states_.clear();

  if (!actions_to_keyname_map_.contains("MouseLeftClick")) {
    LOGT(TAG, "MouseLeftClick has no mapping, add default mapping with 'MouseLeft'.");

    actions_to_keyname_map_["MouseLeftClick"] = {"MouseLeft"};
  }
  if (!actions_to_keyname_map_.contains("MouseRightClick")) {
    LOGT(TAG, "MouseRightClick has no mapping, add default mapping with "
              "'MouseRight'.");

    actions_to_keyname_map_["MouseRightClick"] = {"MouseRight"};
  }
  for (const auto& [action_name, key_names] : actions_to_keyname_map_) {
    action_states_[action_name] = ActionState::kInactive;
    LOGT(TAG, "map action: {}", action_name);
    for (const auto& key_name : key_names) {
      SDL_Scancode scancode = ScancodeFromString(key_name);
      Uint32 mouse_button = MouseButtonFromString(key_name);

      if (scancode != SDL_SCANCODE_UNKNOWN) {
        input_to_actions_map_[scancode].push_back(action_name);
        LOGT(TAG, "  map key: {} (Scancode: {}) to action: {}", key_name, static_cast<int>(scancode), action_name);
      } else if (mouse_button != 0) {
        input_to_actions_map_[mouse_button].push_back(action_name);
        LOGT(TAG, "  map mouse button: {} (Button ID: {}) to action: {}", key_name, static_cast<int>(mouse_button),
             action_name);
      } else {
        LOGW(TAG, "  map unknown key: {} to action: {}", key_name, action_name);
      }
    }
  }
  LOGT(TAG, "Input mappings initialized.");
}

SDL_Scancode InputManager::ScancodeFromString(std::string_view key_name) {
  return SDL_GetScancodeFromName(key_name.data());
}

Uint32 InputManager::MouseButtonFromString(std::string_view button_name) {
  if (button_name == "MouseLeft")
    return SDL_BUTTON_LEFT;
  if (button_name == "MouseMiddle")
    return SDL_BUTTON_MIDDLE;
  if (button_name == "MouseRight")
    return SDL_BUTTON_RIGHT;

  if (button_name == "MouseX1")
    return SDL_BUTTON_X1;
  if (button_name == "MouseX2")
    return SDL_BUTTON_X2;
  return 0;  // 0 is not a valid mouse button value
}

void InputManager::UpdateActionState(std::string_view action_name, bool is_input_active, bool is_repeat_event) {
  auto it = action_states_.find(std::string(action_name));
  if (it == action_states_.end()) {
    spdlog::warn("尝试更新未注册的动作状态: {}", action_name);
    return;
  }

  if (is_input_active) {  // 输入被激活 (按下)
    if (is_repeat_event) {
      it->second = ActionState::kHeldDown;
    } else {  // 非重复的按下事件
      it->second = ActionState::kPressedThisFrame;
    }
  } else {  // 输入被释放 (松开)
    it->second = ActionState::kReleasedThisFrame;
  }
}

}  // namespace engine::input
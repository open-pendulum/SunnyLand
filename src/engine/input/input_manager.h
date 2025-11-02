#pragma once

namespace engine::input {

class InputManager {
 public:
  InputManager();
  ~InputManager();

  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;
  InputManager(InputManager&&) = delete;
  InputManager& operator=(InputManager&&) = delete;

 private:
};
}  // namespace engine::input
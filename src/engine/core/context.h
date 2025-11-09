#pragma once

namespace engine::input {
class InputManager;
}  // namespace engine::input

namespace engine::render {
class Renderer;
class Camera;
}  // namespace engine::render

namespace engine::resource {
class ResourceManager;
}  // namespace engine::resource

namespace engine::core {

class Context final {
 public:
  explicit Context(engine::input::InputManager& input_manager, engine::render::Renderer& renderer,
                   engine::render::Camera& camera, engine::resource::ResourceManager& resource_manager);

  // 禁止拷贝和移动，Context 对象通常是唯一的或按需创建/传递
  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

  [[nodiscard]] engine::input::InputManager& GetInputManager() const {
    return input_manager_;
  }
  [[nodiscard]] engine::render::Renderer& GetRenderer() const {
    return renderer_;
  }
  [[nodiscard]] engine::render::Camera& GetCamera() const {
    return camera_;
  }
  [[nodiscard]] engine::resource::ResourceManager& GetResourceManager() const {
    return resource_manager_;
  }

 private:
  engine::input::InputManager& input_manager_;
  engine::render::Renderer& renderer_;
  engine::render::Camera& camera_;
  engine::resource::ResourceManager& resource_manager_;
};

}  // namespace engine::core
#pragma once
#include <memory>
#include <string>
#include <vector>

namespace engine::core {
class Context;
}

namespace engine::render {
class Renderer;
class Camera;
}  // namespace engine::render

namespace engine::input {
class InputManager;
}

namespace engine::object {
class GameObject;
}

namespace engine::scene {
class SceneManager;

class Scene {
 public:
  explicit Scene(std::string name, engine::core::Context& context, engine::scene::SceneManager& scene_manager);

  virtual ~Scene();

  // 禁止拷贝和移动构造
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
  Scene(Scene&&) = delete;
  Scene& operator=(Scene&&) = delete;

  virtual void Init();
  virtual void Update(double delta_time_s);
  virtual void Render();
  virtual void HandleInput();
  virtual void Clean();

  virtual void AddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

  virtual void SafeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

  virtual void RemoveGameObject(engine::object::GameObject* game_object_ptr);

  virtual void SafeRemoveGameObject(engine::object::GameObject* game_object_ptr);

  [[nodiscard]] const std::vector<std::unique_ptr<engine::object::GameObject>>& GetGameObjects() const {
    return game_objects_;
  }

  [[nodiscard]] engine::object::GameObject* FindGameObjectByName(const std::string& name) const;

  void SetName(const std::string& name) {
    scene_name_ = name;
  }
  [[nodiscard]] const std::string& GetName() const {
    return scene_name_;
  }
  void SetInitialized(bool initialized) {
    is_initialized_ = initialized;
  }
  [[nodiscard]] bool IsInitialized() const {
    return is_initialized_;
  }

  [[nodiscard]] engine::core::Context& GetContext() const {
    return context_;
  }
  [[nodiscard]] engine::scene::SceneManager& GetSceneManager() const {
    return scene_manager_;
  }
  std::vector<std::unique_ptr<engine::object::GameObject>>& GetGameObjects() {
    return game_objects_;
  }

 protected:
  void ProcessPendingAdditions();

 protected:
  std::string scene_name_;
  engine::core::Context& context_;
  engine::scene::SceneManager& scene_manager_;
  bool is_initialized_{false};
  std::vector<std::unique_ptr<engine::object::GameObject>> game_objects_;
  std::vector<std::unique_ptr<engine::object::GameObject>> pending_additions_;
};

}  // namespace engine::scene
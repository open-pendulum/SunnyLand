#pragma once
#include <memory>
#include <string>
#include <vector>

// 前置声明
namespace engine::core {
class Context;
}  // namespace engine::core
namespace engine::scene {
class Scene;
}  // namespace engine::scene

namespace engine::scene {

class SceneManager final {
 public:
  explicit SceneManager(engine::core::Context& context);
  ~SceneManager();

  // 禁止拷贝和移动
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;
  SceneManager(SceneManager&&) = delete;
  SceneManager& operator=(SceneManager&&) = delete;

  void RequestPushScene(std::unique_ptr<Scene>&& scene);
  void RequestPopScene();
  void RequestReplaceScene(std::unique_ptr<Scene>&& scene);

  [[nodiscard]] Scene* GetCurrentScene() const;
  [[nodiscard]] engine::core::Context& GetContext() const {
    return context_;
  }

  void Update(double delta_time_s);
  void Render();
  void HandleInput() const;
  void Close();

 private:
  void ProcessPendingActions();

  void PushScene(std::unique_ptr<Scene>&& scene);
  void PopScene();
  void ReplaceScene(std::unique_ptr<Scene>&& scene);

 private:
  engine::core::Context& context_;
  std::vector<std::unique_ptr<Scene>> scene_stack_;

  enum class PendingAction { None, Push, Pop, Replace };
  PendingAction pending_action_ = PendingAction::None;
  std::unique_ptr<Scene> pending_scene_;
};

}  // namespace engine::scene
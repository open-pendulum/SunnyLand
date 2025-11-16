#pragma once
#include <memory>
#include "scene/scene.h"

namespace engine::object {
class GameObject;
}  // namespace engine::object

namespace game::scene {

class GameScene final : public engine::scene::Scene {
 public:
  explicit GameScene(const std::string& name, engine::core::Context& context,
                     engine::scene::SceneManager& scene_manager);

  void Init() override;
  void Update(double delta_time_s) override;
  void Render() override;
  void HandleInput() override;
  void Clean() override;

 private:
  void CreateTestObject();
};

}  // namespace game::scene
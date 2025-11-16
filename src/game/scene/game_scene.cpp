#include "game_scene.h"
#include <SDL3/SDL_rect.h>
#include "component/sprite_component.h"
#include "component/transform_component.h"
#include "core/context.h"
#include "logger.hpp"
#include "object/game_object.h"

namespace game::scene {
namespace {
DECLARE_TAG(GameScene)
}  // namespace
GameScene::GameScene(const std::string& name, engine::core::Context& context,
                     engine::scene::SceneManager& scene_manager)
    : Scene(name, context, scene_manager) {
  LOGT(TAG, "GameScene constructor");
}

void GameScene::Init() {
  CreateTestObject();

  Scene::Init();
  LOGT(TAG, "GameScene Init");
}

void GameScene::Update(double delta_time_s) {
  Scene::Update(delta_time_s);
}

void GameScene::Render() {
  Scene::Render();
}

void GameScene::HandleInput() {
  Scene::HandleInput();
}

void GameScene::Clean() {
  Scene::Clean();
}

void GameScene::CreateTestObject() {
  LOGT(TAG, "Create test_object...");
  auto test_object = std::make_unique<engine::object::GameObject>("test_object");

  test_object->AddComponent<engine::component::TransformComponent>(glm::vec2(100.0f, 100.0f));
  test_object->AddComponent<engine::component::SpriteComponent>(
      "D:/workspace/SunnyLand/assets/textures/Props/big-crate.png", context_.GetResourceManager());

  AddGameObject(std::move(test_object));
  LOGT(TAG, "test_object created and added to GameScene.");
}

}  // namespace game::scene
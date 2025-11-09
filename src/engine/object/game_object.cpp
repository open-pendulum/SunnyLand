#include "game_object.h"
#include <spdlog/spdlog.h>
#include "input/input_manager.h"
#include "render/camera.h"
#include "render/renderer.h"

namespace engine::object {
GameObject::GameObject(const std::string& name, const std::string& tag) : name_(name), tag_(tag) {
  spdlog::trace("GameObject created: {} {}", name_, tag_);
}

void GameObject::Update(float delta_time_s, engine::core::Context& context) {
  // 遍历所有组件并调用它们的 update 方法
  for (auto& pair : components_) {
    pair.second->Update(delta_time_s, context);
  }
}

void GameObject::Render(engine::core::Context& context) {
  // 遍历所有组件并调用它们的 render 方法
  for (auto& pair : components_) {
    pair.second->Render(context);
  }
}

void GameObject::Clean() {
  spdlog::trace("Cleaning GameObject...");
  // 遍历所有组件并调用它们的 clean 方法
  for (auto& pair : components_) {
    pair.second->Clean();
  }
  components_.clear();  // 清空 map, unique_ptr 会自动释放内存
}

void GameObject::HandleInput(engine::core::Context& context) {
  // 遍历所有组件并调用它们的 handleInput 方法
  for (auto& pair : components_) {
    pair.second->HandleInput(context);
  }
}

}  // namespace engine::object
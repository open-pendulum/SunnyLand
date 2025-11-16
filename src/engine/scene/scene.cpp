#include "scene.h"
#include "logger.hpp"
#include "object/game_object.h"
#include "scene_manager.h"

#include <algorithm>
namespace engine::scene {
namespace {
DECLARE_TAG(Scene);
}  // namespace

Scene::Scene(std::string name, engine::core::Context& context, engine::scene::SceneManager& scene_manager)
    : scene_name_(std::move(name)), context_(context), scene_manager_(scene_manager), is_initialized_(false) {
  LOGI(TAG, "scene {} constructor succeeded", scene_name_);
}

Scene::~Scene() = default;

void Scene::Init() {
  is_initialized_ = true;
  LOGT(TAG, "scene {} initialize succeeded", scene_name_);
}

void Scene::Update(double delta_time_s) {
  if (!is_initialized_)
    return;

  for (auto it = game_objects_.begin(); it != game_objects_.end();) {
    if (*it && !(*it)->IsNeedRemove()) {
      (*it)->Update(delta_time_s, context_);
      ++it;
    } else {
      if (*it) {
        (*it)->Clean();
      }
      it = game_objects_.erase(it);
    }
  }

  ProcessPendingAdditions();
}

void Scene::Render() {
  if (!is_initialized_)
    return;

  for (const auto& obj : game_objects_) {
    if (obj)
      obj->Render(context_);
  }
}

void Scene::HandleInput() {
  if (!is_initialized_)
    return;

  for (auto it = game_objects_.begin(); it != game_objects_.end();) {
    if (*it && !(*it)->IsNeedRemove()) {
      (*it)->HandleInput(context_);
      ++it;
    } else {
      if (*it)
        (*it)->Clean();
      it = game_objects_.erase(it);
    }
  }
}

void Scene::Clean() {
  if (!is_initialized_)
    return;

  for (const auto& obj : game_objects_) {
    if (obj)
      obj->Clean();
  }
  game_objects_.clear();

  is_initialized_ = false;
  LOGT(TAG, "scene {} clean succeeded", scene_name_);
}

void Scene::AddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object) {
  if (game_object) {
    game_objects_.push_back(std::move(game_object));
  } else {
    LOGW(TAG, "try to add null game object to scene {}", scene_name_);
  }
}

void Scene::SafeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object) {
  if (game_object) {
    pending_additions_.push_back(std::move(game_object));
  } else {
    LOGW(TAG, "try to add null game object to scene {}", scene_name_);
  }
}

void Scene::RemoveGameObject(engine::object::GameObject* game_object_ptr) {
  if (!game_object_ptr) {
    LOGW(TAG, "try to remove null game object from scene {}", scene_name_);
    return;
  }

  auto it = std::remove_if(
      game_objects_.begin(), game_objects_.end(),
      [game_object_ptr](const std::unique_ptr<engine::object::GameObject>& p) { return p.get() == game_object_ptr; });

  if (it != game_objects_.end()) {
    (*it)->Clean();
    game_objects_.erase(it, game_objects_.end());
    LOGT(TAG, "remove game obj from scene {}", scene_name_);
  } else {
    LOGW(TAG, "did not find game object from scene {}", scene_name_);
  }
}

void Scene::SafeRemoveGameObject(engine::object::GameObject* game_object_ptr) {
  game_object_ptr->SetNeedRemove(true);
}

engine::object::GameObject* Scene::FindGameObjectByName(const std::string& name) const {
  for (const auto& obj : game_objects_) {
    if (obj && obj->GetName() == name) {
      return obj.get();
    }
  }
  return nullptr;
}

void Scene::ProcessPendingAdditions() {
  for (auto& game_object : pending_additions_) {
    AddGameObject(std::move(game_object));
  }
  pending_additions_.clear();
}

}  // namespace engine::scene
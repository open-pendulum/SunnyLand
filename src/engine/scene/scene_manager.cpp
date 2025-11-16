#include "scene_manager.h"
#include <logger.hpp>
#include "core/context.h"
#include "scene.h"

namespace engine::scene {
namespace {
DECLARE_TAG(SceneManager);
}  // namespace

SceneManager::SceneManager(engine::core::Context& context) : context_(context) {
  LOGT(TAG, "scene_manager_ created.");
}

SceneManager::~SceneManager() {
  LOGT(TAG, "scene_manager_ destroyed.");
  Close();
}

Scene* SceneManager::GetCurrentScene() const {
  if (scene_stack_.empty()) {
    return nullptr;
  }
  return scene_stack_.back().get();
}

void SceneManager::Update(double delta_time_s) {
  if (Scene* current_scene = GetCurrentScene()) {
    current_scene->Update(delta_time_s);
  }

  ProcessPendingActions();
}

void SceneManager::Render() {
  for (const auto& scene : scene_stack_) {
    if (scene) {
      scene->Render();
    }
  }
}

void SceneManager::HandleInput() const {
  if (Scene* current_scene = GetCurrentScene()) {
    current_scene->HandleInput();
  }
}

void SceneManager::Close() {
  LOGT(TAG, "closing scene and cleaning scene stack...");
  while (!scene_stack_.empty()) {
    if (scene_stack_.back()) {
      LOGT(TAG, "cleaning scene '{}' ...", scene_stack_.back()->GetName());
      scene_stack_.back()->Clean();
    }
    scene_stack_.pop_back();
  }
}

void SceneManager::RequestPopScene() {
  pending_action_ = PendingAction::Pop;
}

void SceneManager::RequestReplaceScene(std::unique_ptr<Scene>&& scene) {
  pending_action_ = PendingAction::Replace;
  pending_scene_ = std::move(scene);
}

void SceneManager::RequestPushScene(std::unique_ptr<Scene>&& scene) {
  pending_action_ = PendingAction::Push;
  pending_scene_ = std::move(scene);
}

void SceneManager::ProcessPendingActions() {
  if (pending_action_ == PendingAction::None) {
    return;
  }

  switch (pending_action_) {
  case PendingAction::Pop:
    PopScene();
    break;
  case PendingAction::Replace:
    ReplaceScene(std::move(pending_scene_));
    break;
  case PendingAction::Push:
    PushScene(std::move(pending_scene_));
    break;
  default:
    break;
  }

  pending_action_ = PendingAction::None;
}

void SceneManager::PushScene(std::unique_ptr<Scene>&& scene) {
  if (!scene) {
    LOGW(TAG, "try to push null scene to scene stack.");
    return;
  }
  LOGT(TAG, "pushing scene '{}' ...", scene->GetName());

  if (!scene->IsInitialized()) {
    scene->Init();
  }

  scene_stack_.push_back(std::move(scene));
}

void SceneManager::PopScene() {
  if (scene_stack_.empty()) {
    LOGW(TAG, "try to pop scene from empty scene stack.");
    return;
  }
  LOGT(TAG, "popping scene '{}' ...", scene_stack_.back()->GetName());

  if (scene_stack_.back()) {
    scene_stack_.back()->Clean();
  }
  scene_stack_.pop_back();
}

void SceneManager::ReplaceScene(std::unique_ptr<Scene>&& scene) {
  if (!scene) {
    LOGW(TAG, "try to replace scene with null scene.");
    return;
  }
  LOGT(TAG, "replacing scene '{}' with scene '{}' ...", scene_stack_.back()->GetName(), scene->GetName());

  while (!scene_stack_.empty()) {
    if (scene_stack_.back()) {
      scene_stack_.back()->Clean();
    }
    scene_stack_.pop_back();
  }

  if (!scene->IsInitialized()) {
    scene->Init();
  }

  scene_stack_.push_back(std::move(scene));
}

}  // namespace engine::scene
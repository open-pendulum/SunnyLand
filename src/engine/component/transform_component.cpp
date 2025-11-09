#include "transform_component.h"
#include "object/game_object.h"
#include "sprite_component.h"

namespace engine::component {
TransformComponent::TransformComponent(glm::vec2 position, glm::vec2 scale, float rotation)
    : position_(position), scale_(scale), rotation_(rotation) {
}

void TransformComponent::SetScale(const glm::vec2& scale) {
  scale_ = scale;
  if (owner_) {
    if (auto sprite_comp = owner_->GetComponent<SpriteComponent>()) {
      sprite_comp->UpdateOffset();
    }
  }
}

}  // namespace engine::component
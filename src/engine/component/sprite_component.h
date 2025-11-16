#pragma once
#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>
#include <optional>
#include <string>
#include "component.h"
#include "render/sprite.h"
#include "utils/alignment.h"

namespace engine::core {
class Context;
}  // namespace engine::core

namespace engine::resource {
class ResourceManager;
}  // namespace engine::resource

namespace engine::component {
class TransformComponent;

class SpriteComponent final : public engine::component::Component {
  friend class engine::object::GameObject;

 public:
  explicit SpriteComponent(const std::string& texture_id, engine::resource::ResourceManager& resource_manager,
                           engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
                           std::optional<SDL_FRect> source_rect_opt = std::nullopt, bool is_flipped = false);
  ~SpriteComponent() override = default;

  // 禁止拷贝和移动
  SpriteComponent(const SpriteComponent&) = delete;
  SpriteComponent& operator=(const SpriteComponent&) = delete;
  SpriteComponent(SpriteComponent&&) = delete;
  SpriteComponent& operator=(SpriteComponent&&) = delete;

  void UpdateOffset();

  // Getters
  [[nodiscard]] const engine::render::Sprite& GetSprite() const {
    return sprite_;
  }
  [[nodiscard]] const std::string& GetTextureId() const {
    return sprite_.GetTextureId();
  }
  [[nodiscard]] bool IsFlipped() const {
    return sprite_.IsFlipped();
  }
  [[nodiscard]] bool IsHidden() const {
    return is_hidden_;
  }
  [[nodiscard]] const glm::vec2& GetSpriteSize() const {
    return sprite_size_;
  }
  [[nodiscard]] const glm::vec2& GetOffset() const {
    return offset_;
  }
  [[nodiscard]] engine::utils::Alignment GetAlignment() const {
    return alignment_;
  }

  // Setters
  void SetSpriteById(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect_opt = std::nullopt);
  void SetFlipped(bool flipped) {
    sprite_.SetFlipped(flipped);
  }
  void SetHidden(bool hidden) {
    is_hidden_ = hidden;
  }
  void SetSourceRect(const std::optional<SDL_FRect>& source_rect_opt);
  void SetAlignment(engine::utils::Alignment anchor);

 private:
  void UpdateSpriteSize();

  // Component 虚函数覆盖
  void Init() override;
  void Update(double delta_time_s, engine::core::Context& context) override {
  }
  void Render(engine::core::Context& context) override;

 private:
  engine::resource::ResourceManager* resource_manager_ = nullptr;
  TransformComponent* transform_ = nullptr;

  engine::render::Sprite sprite_;
  engine::utils::Alignment alignment_ = engine::utils::Alignment::NONE;
  glm::vec2 sprite_size_ = {0.0f, 0.0f};
  glm::vec2 offset_ = {0.0f, 0.0f};
  bool is_hidden_ = false;
};

}  // namespace engine::component
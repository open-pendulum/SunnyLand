// renderer.h
#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include "sprite.h"

struct SDL_Renderer;
struct SDL_FRect;

namespace engine::resource {
class ResourceManager;
}

namespace engine::render {
class Camera;

class Renderer final {
 public:
  explicit Renderer(SDL_Renderer* sdl_renderer, engine::resource::ResourceManager* resource_manager);

  void DrawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position,
                  const glm::vec2& scale = {1.0f, 1.0f}, double angle = 0.0f) const;
  void DrawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position,
                    const glm::vec2& scroll_factor, const glm::bvec2& repeat = {true, true},
                    const glm::vec2& scale = {1.0f, 1.0f}) const;

  void DrawUISprite(const Sprite& sprite, const glm::vec2& position,
                    const std::optional<glm::vec2>& size = std::nullopt) const;

  void Present() const;
  void ClearScreen() const;

  void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
  void SetDrawColorFloat(float r, float g, float b, float a = 1.0f) const;

  SDL_Renderer* GetSDLRenderer() const {
    return renderer_;
  }

  // 禁用拷贝和移动语义
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

 private:
  std::optional<SDL_FRect> GetSpriteSrcRect(const Sprite& sprite) const;
  [[nodiscard]] bool IsRectInViewport(const Camera& camera, const SDL_FRect& rect) const;

 private:
  SDL_Renderer* renderer_ = nullptr;
  engine::resource::ResourceManager* resource_manager_ = nullptr;
};

}  // namespace engine::render

#include "renderer.h"

#include <SDL3/SDL_render.h>

#include "camera.h"
#include "logger.hpp"
#include "resource/resource_manager.h"
namespace engine::render {
namespace {
DECLARE_TAG(Renderer);
}  // namespace
Renderer::Renderer(SDL_Renderer* sdl_renderer, engine::resource::ResourceManager* resource_manager)
    : renderer_(sdl_renderer), resource_manager_(resource_manager) {
  TRACEI(TAG);
  if (renderer_ == nullptr) {
    throw std::runtime_error("SDL_Renderer could not initialize");
  }
  if (resource_manager_ == nullptr) {
    throw std::runtime_error("ResourceManager could not initialize");
  }
}

void Renderer::DrawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale,
                          double angle) const {
  auto texture = resource_manager_->GetTexture(sprite.GetTextureId());
  if (texture == nullptr) {
    LOGE(TAG, "Failed to get texture for {}!", sprite.GetTextureId());
    return;
  }

  auto src_rect = GetSpriteSrcRect(sprite);
  if (!src_rect.has_value()) {
    LOGE(TAG, "Failed to get source rect for {}!", sprite.GetTextureId());
    return;
  }
  glm::vec2 position_screen = camera.WorldToScreen(position);
  float scale_w = src_rect.value().w * scale.x;
  float scale_h = src_rect.value().h * scale.y;
  SDL_FRect dst_rect = {position_screen.x, position_screen.y, scale_w, scale_h};
  if (!IsRectInViewport(camera, dst_rect)) {
    return;
  }
  if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dst_rect, angle, nullptr,
                                sprite.IsFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL)) {
    LOGE(TAG, "Failed to render texture for {}!", sprite.GetTextureId());
    return;
  }
}
void Renderer::DrawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position,
                            const glm::vec2& scroll_factor, const glm::bvec2& repeat, const glm::vec2& scale) const {
  const auto texture = resource_manager_->GetTexture(sprite.GetTextureId());
  if (texture == nullptr) {
    LOGE(TAG, "Failed to get texture for {}!", sprite.GetTextureId());
    return;
  }

  const auto src_rect = GetSpriteSrcRect(sprite);
  if (!src_rect.has_value()) {
    LOGE(TAG, "Failed to get source rect for {}!", sprite.GetTextureId());
    return;
  }

  // 应用相机变换
  const glm::vec2 position_screen = camera.WorldToScreenWithParallax(position, scroll_factor);

  // 计算缩放后的纹理尺寸
  const float scaled_tex_w = src_rect.value().w * scale.x;
  const float scaled_tex_h = src_rect.value().h * scale.y;

  glm::vec2 start, stop;
  const glm::vec2 viewport_size = camera.GetViewportSize();

  if (repeat.x) {
    start.x = glm::mod(position_screen.x, scaled_tex_w) - scaled_tex_w;
    stop.x = viewport_size.x;
  } else {
    start.x = position_screen.x;
    stop.x = glm::min(position_screen.x + scaled_tex_w, viewport_size.x);
  }
  if (repeat.y) {
    start.y = glm::mod(position_screen.y, scaled_tex_h) - scaled_tex_h;
    stop.y = viewport_size.y;
  } else {
    start.y = position_screen.y;
    stop.y = glm::min(position_screen.y + scaled_tex_h, viewport_size.y);
  }

  for (float y = start.y; y < stop.y; y += scaled_tex_h) {
    for (float x = start.x; x < stop.x; x += scaled_tex_w) {
      SDL_FRect dest_rect = {x, y, scaled_tex_w, scaled_tex_h};
      if (!SDL_RenderTexture(renderer_, texture, nullptr, &dest_rect)) {
        LOGE(TAG, "Failed to render texture for {}!", sprite.GetTextureId());
        return;
      }
    }
  }
}
void Renderer::DrawUISprite(const Sprite& sprite, const glm::vec2& position,
                            const std::optional<glm::vec2>& size) const {
  const auto texture = resource_manager_->GetTexture(sprite.GetTextureId());
  if (texture == nullptr) {
    LOGE(TAG, "Failed to get texture for {}!", sprite.GetTextureId());
    return;
  }
  const auto src_rect = GetSpriteSrcRect(sprite);
  if (!src_rect.has_value()) {
    LOGE(TAG, "Failed to get source rect for {}!", sprite.GetTextureId());
    return;
  }

  SDL_FRect dest_rect = {position.x, position.y, 0, 0};
  if (size.has_value()) {
    dest_rect.w = size.value().x;
    dest_rect.h = size.value().y;
  } else {
    dest_rect.w = src_rect.value().w;
    dest_rect.h = src_rect.value().h;
  }

  if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dest_rect, 0.0, nullptr,
                                sprite.IsFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) {
    LOGE(TAG, "Failed to render texture for {}!", sprite.GetTextureId());
  }
}
void Renderer::Present() const {
  SDL_RenderPresent(renderer_);
}
void Renderer::ClearScreen() const {
  if (!SDL_RenderClear(renderer_)) {
    LOGE(TAG, "SDL_RenderClear failed: {}!", SDL_GetError());
  }
}
void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
  if (!SDL_SetRenderDrawColor(renderer_, r, g, b, a)) {
    LOGE(TAG, "Failed to set draw color: {}!", SDL_GetError());
  }
}
void Renderer::SetDrawColorFloat(float r, float g, float b, float a) const {
  if (!SDL_SetRenderDrawColorFloat(renderer_, r, g, b, a)) {
    LOGE(TAG, "Failed to set draw color: {}!", SDL_GetError());
  }
}
std::optional<SDL_FRect> Renderer::GetSpriteSrcRect(const Sprite& sprite) const {
  const auto texture = resource_manager_->GetTexture(sprite.GetTextureId());
  if (texture == nullptr) {
    LOGE(TAG, "Failed to get texture for {}!", sprite.GetTextureId());
    return std::nullopt;
  }

  const auto src_rect = sprite.GetSourceRect();
  if (src_rect.has_value()) {
    if (src_rect.value().w <= 0 || src_rect.value().h <= 0) {
      LOGE(TAG, "Invalid source rect for {}! w: {}, h: {}", sprite.GetTextureId(), src_rect.value().w,
           src_rect.value().h);
      return std::nullopt;
    }
    return src_rect;
  } else {
    SDL_FRect result = {0, 0, 0, 0};
    if (!SDL_GetTextureSize(texture, &result.w, &result.h)) {
      LOGE(TAG, "Failed to get texture for {}!", sprite.GetTextureId());
      return std::nullopt;
    }
    return result;
  }
}
bool Renderer::IsRectInViewport(const Camera& camera, const SDL_FRect& rect) const {
  glm::vec2 viewport_size = camera.GetViewportSize();
  return rect.x + rect.w >= 0 && rect.x <= viewport_size.x && rect.y + rect.h >= 0 && rect.y <= viewport_size.y;
}
}  // namespace engine::render

#include "sprite.h"

namespace engine::render {
Sprite::Sprite(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect, bool is_flipped)
    : texture_id_(texture_id), source_rect_(source_rect), is_flipped_(is_flipped) {
}
Sprite::~Sprite() {
}
const std::string& Sprite::GetTextureId() const {
  return texture_id_;
}
const std::optional<SDL_FRect>& Sprite::GetSourceRect() const {
  return source_rect_;
}
bool Sprite::IsFlipped() const {
  return is_flipped_;
}
void Sprite::SetTextureId(const std::string& texture_id) {
  texture_id_ = texture_id;
}
void Sprite::SetSourceRect(const std::optional<SDL_FRect>& source_rect) {
  source_rect_ = source_rect;
}
void Sprite::SetFlipped(bool flipped) {
  is_flipped_ = flipped;
}
}  // namespace engine::render

#pragma once

#include <SDL3/SDL_rect.h>
#include <optional>
#include <string>

namespace engine::render {
class Sprite final {
 public:
  explicit Sprite(const std::string& texture_id,
                  const std::optional<SDL_FRect>& source_rect = std::nullopt,
                  bool is_flipped = false);
  ~Sprite();

  const std::string& GetTextureId() const;
  const std::optional<SDL_FRect>& GetSourceRect() const;
  bool IsFlipped() const;

  void SetTextureId(const std::string& texture_id);
  void SetSourceRect(const std::optional<SDL_FRect>& source_rect);
  void SetIsFlipped(bool is_flipped);

 private:
  std::string texture_id_;
  std::optional<SDL_FRect> source_rect_;
  bool is_flipped_ = true;
};
}  // namespace engine::render

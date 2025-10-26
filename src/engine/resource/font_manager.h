#pragma once

namespace engine::resource {
class FontManager {
 public:
  FontManager();

  FontManager(const FontManager& other) = delete;
  FontManager& operator=(const FontManager& other) = delete;
  FontManager(FontManager&& other) = delete;
  FontManager& operator=(FontManager&& other) = delete;

  ~FontManager();
};
}  // namespace engine::resource

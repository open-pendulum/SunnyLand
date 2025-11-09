#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine::resource {

class FontManager final {
 public:
  FontManager();

  FontManager(const FontManager& other) = delete;
  FontManager& operator=(const FontManager& other) = delete;
  FontManager(FontManager&& other) = delete;
  FontManager& operator=(FontManager&& other) = delete;

  ~FontManager();

 public:
  TTF_Font* LoadFont(const std::string& file_path, int font_size);
  TTF_Font* GetFont(const std::string& file_path, int font_size);
  void UnloadFont(const std::string& file_path, int font_size);
  void ClearFonts();

 private:
  struct SDLFontDeleter {
    void operator()(TTF_Font* font) const {
      if (font) {
        TTF_CloseFont(font);
      }
    }
  };

  using FontKey = std::pair<std::string, int32_t>;
  struct FontKeyHash {
    std::size_t operator()(const FontKey& key) const {
      return std::hash<std::string>()(key.first) ^ std::hash<int32_t>()(key.second);
    }
  };
  std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> fonts_;
};
}  // namespace engine::resource

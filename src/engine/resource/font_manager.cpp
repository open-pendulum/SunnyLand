#include "font_manager.h"
#include "logger.hpp"

namespace engine::resource {
namespace {
DECLARE_TAG(FontManager);
}

FontManager::FontManager() {
  TRACEI(TAG);
  if (!TTF_WasInit() && !TTF_Init()) {
    throw std::runtime_error("TTF_Init() failed");
  }
}
FontManager::~FontManager() {
  TRACEI(TAG);
  ClearFonts();
}
TTF_Font* FontManager::LoadFont(const std::string& file_path, int font_size) {
  if (font_size <= 0) {
    LOGE(TAG, "font_size must be greater than 0");
    return nullptr;
  }
  const FontKey key = {file_path, font_size};
  if (fonts_.contains(key)) {
    return fonts_.at(key).get();
  }
  TTF_Font* font = TTF_OpenFont(file_path.c_str(), static_cast<float>(font_size));
  if (font == nullptr) {
    LOGE(TAG, "Failed to load font: {}, font_size: {}", file_path, font_size);
    return nullptr;
  }
  fonts_.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(font));
  LOGI(TAG, "Loaded font: {}, font_size: {}", file_path, font_size);
  return font;
}
TTF_Font* FontManager::GetFont(const std::string& file_path, int font_size) {
  const FontKey key = {file_path, font_size};
  if (fonts_.contains(key)) {
    return fonts_.at(key).get();
  }
  LOGW(TAG, "Font not found: {}, font_size: {}, try to load it", file_path,
       font_size);
  return LoadFont(file_path, font_size);
}
void FontManager::UnloadFont(const std::string& file_path, int font_size) {
  const FontKey key = {file_path, font_size};
  if (fonts_.contains(key)) {
    fonts_.erase(key);
    LOGI(TAG, "Unloaded font: {}, font_size: {}", file_path, font_size);
  } else {
    LOGW(TAG, "Font not found: {}, font_size: {}, try to unload it", file_path,
         font_size);
  }
}
void FontManager::ClearFonts() {
  TRACEI(TAG);
  fonts_.clear();
  LOGI(TAG, "Cleared all fonts");
}
}  // namespace engine::resource
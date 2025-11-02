#include "texture_manager.h"
#include "logger.hpp"

#include <SDL3_image/SDL_image.h>

namespace engine::resource {
namespace {
DECLARE_TAG(TextureManager);
}

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer_(renderer) {
  TRACEI(TAG);
  if (renderer_ == nullptr) {
    throw std::invalid_argument("SDL_Renderer is null");
  }
}
TextureManager::~TextureManager() = default;
SDL_Texture* TextureManager::LoadTexture(const std::string& file_path) {
  if (textures_.contains(file_path)) {
    return textures_.at(file_path).get();
  }

  SDL_Texture* raw_texture = IMG_LoadTexture(renderer_, file_path.c_str());
  if (raw_texture == nullptr) {
    LOGE(TAG, "Failed to load texture: {}", file_path);
    return nullptr;
  }
  textures_.emplace(
      file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(raw_texture));
  LOGI(TAG, "Loaded texture: {}", file_path);
  return raw_texture;
}

SDL_Texture* TextureManager::GetTexture(const std::string& file_path) {
  if (textures_.contains(file_path)) {
    return textures_.at(file_path).get();
  }
  LOGW(TAG, "Texture not found: {}, try to load it", file_path);
  return LoadTexture(file_path);
}

void TextureManager::UnloadTexture(const std::string& file_path) {
  if (textures_.contains(file_path)) {
    textures_.erase(file_path);
    LOGI(TAG, "Unloaded texture: {}", file_path);
  } else {
    LOGW(TAG, "Texture not found: {}, cannot unload", file_path);
  }
}
void TextureManager::ClearTextures() {
  textures_.clear();
  LOGI(TAG, "Cleared all textures");
}
glm::vec2 TextureManager::GetTextureSize(const std::string& file_path) {
  const auto texture = GetTexture(file_path);
  if (texture == nullptr) {
    LOGW(TAG, "Texture not found: {}, cannot get size", file_path);
    return glm::vec2(0.0f);
  }
  glm::vec2 size;
  SDL_GetTextureSize(texture, &size.x, &size.y);
  return size;
}
}  // namespace engine::resource

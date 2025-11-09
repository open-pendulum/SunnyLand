#pragma once
#include <SDL3/SDL_render.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

struct SDL_Texture;

namespace engine::resource {

class TextureManager final {
 public:
  explicit TextureManager(SDL_Renderer* renderer);

  TextureManager(const TextureManager& other) = delete;
  TextureManager& operator=(const TextureManager& other) = delete;
  TextureManager(TextureManager&& other) = delete;
  TextureManager& operator=(TextureManager&& other) = delete;

  ~TextureManager();

 public:
  SDL_Texture* LoadTexture(const std::string& file_path);
  SDL_Texture* GetTexture(const std::string& file_path);
  void UnloadTexture(const std::string& file_path);
  void ClearTextures();
  glm::vec2 GetTextureSize(const std::string& file_path);

 private:
  struct SDLTextureDeleter {
    void operator()(SDL_Texture* texture) const {
      if (texture) {
        SDL_DestroyTexture(texture);
      }
    }
  };
  std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> textures_;
  SDL_Renderer* renderer_;
};
}  // namespace engine::resource

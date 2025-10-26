#pragma once
#include <SDL3/SDL_render.h>

struct SDL_Texture;

namespace engine::resource {
struct SDLTextureDeleter {
  void operator()(SDL_Texture* texture) const {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }
};

class TextureManager {
 public:
  TextureManager();

  TextureManager(const TextureManager& other) = delete;
  TextureManager& operator=(const TextureManager& other) = delete;
  TextureManager(TextureManager&& other) = delete;
  TextureManager& operator=(TextureManager&& other) = delete;

  ~TextureManager();
};
}  // namespace engine::resource

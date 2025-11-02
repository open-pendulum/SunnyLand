#include "game_app.h"
#include "logger.hpp"
#include "render/camera.h"
#include "render/renderer.h"
#include "render/sprite.h"
#include "resource/resource_manager.h"
#include "time.h"

#include <SDL3/SDL.h>

namespace engine::core {
namespace {
DECLARE_TAG(GameApp)
}  // namespace
GameApp::GameApp() {
  TRACEI(TAG);
}
GameApp::~GameApp() {
  TRACEI(TAG);
  if (is_running_) {
    resource_manager_.reset();
    Close();
  }
}
void GameApp::Run() {
  TRACEI(TAG);
  if (!Init()) {
    LOGE(TAG, "Failed to initialize!");
    return;
  }
  LOGI(TAG, "Running...");
  time_->SetTargetFPS(144);
  while (is_running_) {
    time_->Update();
    HandleEvents();
    Update(time_->GetDeltaTimeS());
    Render();
  }
}
bool GameApp::Init() {
  TRACEI(TAG);
  if (!InitSDL()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  if (!InitTime()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  if (!InitResourceManager()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  if (!InitRenderer()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  if (!InitCamera()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  {
    // test resource
    resource_manager_->GetTexture("../assets/textures/Actors/frog.png");
    resource_manager_->GetTexture("../assets/textures/UI/buttons/Start1.png");
    resource_manager_->GetTexture("../assets/textures/Layers/back.png");
  }
  is_running_ = true;
  return true;
}

void GameApp::Update(double delta_time_s) {
  {
    // test camera
    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_UP])
      camera_->Move(glm::vec2(0, -1));
    if (key_state[SDL_SCANCODE_DOWN])
      camera_->Move(glm::vec2(0, 1));
    if (key_state[SDL_SCANCODE_LEFT])
      camera_->Move(glm::vec2(-1, 0));
    if (key_state[SDL_SCANCODE_RIGHT])
      camera_->Move(glm::vec2(1, 0));
  }
}

void GameApp::Render() {
  renderer_->ClearScreen();
  {
    // test render
    engine::render::Sprite sprite_world("../assets/textures/Actors/frog.png");
    engine::render::Sprite sprite_ui(
        "../assets/textures/UI/buttons/Start1.png");
    engine::render::Sprite sprite_parallax(
        "../assets/textures/Layers/back.png");

    static float rotation = 0.0f;
    rotation += 0.1f;

    // 注意渲染顺序
    renderer_->DrawParallax(*camera_, sprite_parallax, glm::vec2(100, 100),
                            glm::vec2(0.5f, 0.5f), glm::bvec2(true, false));
    renderer_->DrawSprite(*camera_, sprite_world, glm::vec2(200, 200),
                          glm::vec2(1.0f, 1.0f), rotation);
    renderer_->DrawUISprite(sprite_ui, glm::vec2(100, 100));
  }
  renderer_->Present();
}
void GameApp::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      is_running_ = false;
    }
  }
}
void GameApp::Close() {
  TRACEI(TAG);
  if (sdl_renderer_) {
    SDL_DestroyRenderer(sdl_renderer_);
    sdl_renderer_ = nullptr;
  }

  if (sdl_window_) {
    SDL_DestroyWindow(sdl_window_);
    sdl_window_ = nullptr;
  }
  SDL_Quit();
  is_running_ = false;
}
bool GameApp::InitSDL() {
  TRACEI(TAG);
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    LOGE(TAG, "Failed to initialize! SDL Error: {}", SDL_GetError());
    return false;
  }

  sdl_window_ = SDL_CreateWindow("SunnyLand", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (!sdl_window_) {
    LOGE(TAG, "Failed to create window! SDL Error: {}", SDL_GetError());
    return false;
  }

  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, nullptr);
  if (!sdl_renderer_) {
    LOGE(TAG, "Failed to create renderer! SDL Error: %s", SDL_GetError());
    return false;
  }
  SDL_SetRenderLogicalPresentation(sdl_renderer_, 640, 360,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  return true;
}
bool GameApp::InitResourceManager() {
  TRACEI(TAG);
  try {
    resource_manager_ =
        std::make_unique<engine::resource::ResourceManager>(sdl_renderer_);
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize ResourceManager! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitTime() {
  TRACEI(TAG);
  try {
    time_ = std::make_unique<Time>();
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Time! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitRenderer() {
  TRACEI(TAG);
  try {
    renderer_ = std::make_unique<engine::render::Renderer>(
        sdl_renderer_, resource_manager_.get());
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Renderer! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitCamera() {
  TRACEI(TAG);
  try {
    camera_ =
        std::make_unique<engine::render::Camera>(glm::vec2(640.0f, 360.0f));
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Camera! Error: {}", e.what());
    return false;
  }
  return true;
}
}  // namespace engine::core

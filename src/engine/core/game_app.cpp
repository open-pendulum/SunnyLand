#include "game_app.h"
#include "config.h"
#include "context.h"
#include "input/input_manager.h"
#include "logger.hpp"
#include "object/game_object.h"
#include "render/camera.h"
#include "render/renderer.h"
#include "render/sprite.h"
#include "resource/resource_manager.h"
#include "component/transform_component.h"
#include "component/sprite_component.h"
#include "time.h"

#include <SDL3/SDL.h>

namespace engine::core {
namespace {
DECLARE_TAG(GameApp)
engine::object::GameObject game_object("test_game_object");

void TestGameObject() {

}

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
    double delta_time_s = time_->GetDeltaTimeS();

    input_manager_->Update();
    HandleEvents();
    Update(delta_time_s);
    Render();
  }
}
bool GameApp::Init() {
  TRACEI(TAG);
  if (!InitConfig()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }

  if (!InitSDL()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }

  if (!InitInputManager()) {
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
  if (!InitContext()) {
    LOGE(TAG, "Failed to initialize!");
    return false;
  }
  {
    spdlog::info("========== 测试组件系统 ==========");

    // 1. 添加 TransformComponent，让对象出现在 (100, 100)
    game_object.AddComponent<engine::component::TransformComponent>(
        glm::vec2(100.0f, 100.0f)
    );

    // 2. 添加 SpriteComponent，显示箱子贴图，设置渲染中心为图片的几何中心
    game_object.AddComponent<engine::component::SpriteComponent>(
        "D:/workspace/SunnyLand/assets/textures/Props/big-crate.png",
        *resource_manager_,
        engine::utils::Alignment::BOTTOM_CENTER
    );

    // 3. 获取 TransformComponent，修改缩放和旋转
    game_object.GetComponent<engine::component::TransformComponent>()->SetScale(glm::vec2(2.0f, 2.0f));  // 放大 2 倍
    game_object.GetComponent<engine::component::TransformComponent>()->SetRotation(30.0f);                // 旋转 30 度
    spdlog::info("✓ Transform 组件配置完成");

    spdlog::info("====================================");
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
    engine::render::Sprite sprite_world("D:/workspace/SunnyLand/assets/textures/Actors/frog.png");
    engine::render::Sprite sprite_ui("D:/workspace/SunnyLand/assets/textures/UI/buttons/Start1.png");
    engine::render::Sprite sprite_parallax("D:/workspace/SunnyLand/assets/textures/Layers/back.png");

    static float rotation = 0.0f;
    rotation += 0.1f;

    // 注意渲染顺序
    renderer_->DrawParallax(*camera_, sprite_parallax, glm::vec2(100, 100), glm::vec2(0.5f, 0.5f),
                            glm::bvec2(true, false));
    renderer_->DrawSprite(*camera_, sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), rotation);
    renderer_->DrawUISprite(sprite_ui, glm::vec2(100, 100));
  }
  game_object.Render(*context_);
  renderer_->Present();
}
void GameApp::HandleEvents() {
  if (input_manager_->ShouldQuit()) {
    LOGI(TAG, "Quitting!");
    is_running_ = false;
    return;
  }
  {
    std::vector<std::string> actions = {"move_up", "move_down", "move_left",      "move_right",     "jump",
                                        "attack",  "pause",     "MouseLeftClick", "MouseRightClick"};

    for (const auto& action : actions) {
      if (input_manager_->IsActionPressed(action)) {
        spdlog::info(" {} 按下 ", action);
      }
      if (input_manager_->IsActionReleased(action)) {
        spdlog::info(" {} 抬起 ", action);
      }
      if (input_manager_->IsActionDown(action)) {
        spdlog::info(" {} 按下中 ", action);
      }
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

  sdl_window_ = SDL_CreateWindow(config_->WindowTitle().c_str(), config_->WindowWidth(), config_->WindowHeight(),
                                 SDL_WINDOW_RESIZABLE);
  if (!sdl_window_) {
    LOGE(TAG, "Failed to create window! SDL Error: {}", SDL_GetError());
    return false;
  }

  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, nullptr);
  if (!sdl_renderer_) {
    LOGE(TAG, "Failed to create renderer! SDL Error: %s", SDL_GetError());
    return false;
  }

  SDL_SetRenderLogicalPresentation(sdl_renderer_, config_->WindowWidth() / 2, config_->WindowHeight() / 2,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  return true;
}
bool GameApp::InitResourceManager() {
  TRACEI(TAG);
  try {
    resource_manager_ = std::make_unique<engine::resource::ResourceManager>(sdl_renderer_);
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
    time_->SetTargetFPS(config_->TargetFps());
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Time! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitRenderer() {
  TRACEI(TAG);
  try {
    renderer_ = std::make_unique<engine::render::Renderer>(sdl_renderer_, resource_manager_.get());
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Renderer! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitCamera() {
  TRACEI(TAG);
  try {
    camera_ = std::make_unique<engine::render::Camera>(
        glm::vec2(config_->WindowWidth() / 2.0f, config_->WindowHeight() / 2.0f));
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Camera! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitConfig() {
  TRACEI(TAG);
  config_ = std::make_unique<Config>("D:/workspace/SunnyLand/assets/config.json");
  return true;
}
bool GameApp::InitInputManager() {
  try {
    input_manager_ = std::make_unique<engine::input::InputManager>(sdl_renderer_, config_.get());
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize InputManager! Error: {}", e.what());
    return false;
  }
  return true;
}
bool GameApp::InitContext() {
  try {
    context_ = std::make_unique<Context>(*input_manager_, *renderer_, *camera_, *resource_manager_);
  } catch (const std::exception& e) {
    LOGE(TAG, "Failed to initialize Context! Error: {}", e.what());
    return false;
  }
  return true;
}
}  // namespace engine::core

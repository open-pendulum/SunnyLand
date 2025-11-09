#include "context.h"
#include "input/input_manager.h"
#include "logger.hpp"
#include "render/camera.h"
#include "render/renderer.h"
#include "resource/resource_manager.h"

namespace engine::core {

Context::Context(engine::input::InputManager& input_manager, engine::render::Renderer& renderer,
                 engine::render::Camera& camera, engine::resource::ResourceManager& resource_manager)
    : input_manager_(input_manager), renderer_(renderer), camera_(camera), resource_manager_(resource_manager) {
  TRACEI("Context");
}

}  // namespace engine::core
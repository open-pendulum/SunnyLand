#include "camera.h"
#include "logger.hpp"

namespace engine::render {
namespace {
DECLARE_TAG(Camera);
}  // namespace
Camera::Camera(const glm::vec2& viewport_size, const glm::vec2& position,
               const std::optional<engine::utils::Rect>& limit_bounds)
    : viewport_size_(viewport_size), position_(position), limit_bounds_(limit_bounds) {
  LOGI(TAG, "Initialize success, position: {}x{}", position_.x, position_.y);
}
void Camera::Update(float delta_time_s) {
}
void Camera::Move(const glm::vec2& offset) {
  position_ += offset;
  ClampPosition();
}
glm::vec2 Camera::WorldToScreen(const glm::vec2& world_pos) const {
  return world_pos - position_;
}
glm::vec2 Camera::WorldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scroll_factor) const {
  return world_pos - position_ * scroll_factor;
}
glm::vec2 Camera::ScreenToWorld(const glm::vec2& screen_pos) const {
  return screen_pos + position_;
}
void Camera::SetPosition(const glm::vec2& position) {
  position_ = position;
  ClampPosition();
}
void Camera::SetLimitBounds(const engine::utils::Rect& bounds) {
  limit_bounds_ = bounds;
  ClampPosition();
}
const glm::vec2& Camera::GetPosition() const {
  return position_;
}
std::optional<engine::utils::Rect> Camera::GetLimitBounds() const {
  return limit_bounds_;
}
glm::vec2 Camera::GetViewportSize() const {
  return viewport_size_;
}
void Camera::ClampPosition() {
  if (limit_bounds_.has_value() && limit_bounds_->size.x > 0 && limit_bounds_->size.y > 0) {
    glm::vec2 min_position = limit_bounds_->position;
    glm::vec2 max_position = limit_bounds_->position + limit_bounds_->size - viewport_size_;
    max_position.x = std::max(min_position.x, viewport_size_.x);
    max_position.y = std::max(min_position.y, viewport_size_.y);
    position_ = glm::clamp(position_, min_position, max_position);
  }
}
}  // namespace engine::render

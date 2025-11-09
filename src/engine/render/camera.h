// camera.h
#pragma once
#include <optional>
#include "utils/math.hpp"

namespace engine::render {

/**
 * @brief 相机类负责管理相机位置和视口大小，并提供坐标转换功能。
 * 它还包含限制相机移动范围的边界。
 */
class Camera final {
 public:
  explicit Camera(const glm::vec2& viewport_size, const glm::vec2& position = glm::vec2(0.0f, 0.0f),
                  const std::optional<engine::utils::Rect>& limit_bounds = std::nullopt);

  void Update(float delta_time_s);
  void Move(const glm::vec2& offset);

  glm::vec2 WorldToScreen(const glm::vec2& world_pos) const;
  glm::vec2 WorldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scroll_factor) const;
  glm::vec2 ScreenToWorld(const glm::vec2& screen_pos) const;

  void SetPosition(const glm::vec2& position);
  void SetLimitBounds(const engine::utils::Rect& bounds);

  const glm::vec2& GetPosition() const;
  std::optional<engine::utils::Rect> GetLimitBounds() const;
  glm::vec2 GetViewportSize() const;

  // 禁用拷贝和移动语义
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;
  Camera(Camera&&) = delete;
  Camera& operator=(Camera&&) = delete;

 private:
  void ClampPosition();

 private:
  glm::vec2 viewport_size_;
  glm::vec2 position_;
  std::optional<engine::utils::Rect> limit_bounds_;
};

}  // namespace engine::render

#pragma once
#include "component/component.h"
#include "logger.hpp"

#include <spdlog/spdlog.h>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace engine::core {
class Context;
}  // namespace engine::core

namespace engine::object {
namespace {
DECLARE_TAG(GameObject);
}  // namespace

class GameObject final {
 public:
  explicit GameObject(const std::string& name = "", const std::string& tag = "");

  // 禁止拷贝和移动，确保唯一性 (通常游戏对象不应随意拷贝)
  GameObject(const GameObject&) = delete;
  GameObject& operator=(const GameObject&) = delete;
  GameObject(GameObject&&) = delete;
  GameObject& operator=(GameObject&&) = delete;

  // setters and getters
  void SetName(const std::string& name) {
    name_ = name;
  }
  const std::string& GetName() const {
    return name_;
  }
  void SetTag(const std::string& tag) {
    tag_ = tag;
  }
  const std::string& GetTag() const {
    return tag_;
  }
  void SetNeedRemove(bool need_remove) {
    need_remove_ = need_remove;
  }
  bool IsNeedRemove() const {
    return need_remove_;
  }

  template <typename T, typename... Args>
  T* AddComponent(Args&&... args) {
    // 检测组件是否合法。  /*  static_assert(condition, message)：静态断言，在编译期检测，无任何性能影响 */
    /* std::is_base_of<Base, Derived>::value -- 判断 Base 类型是否是 Derived 类型的基类 */
    static_assert(std::is_base_of_v<engine::component::Component, T>, "T 必须继承自 Component");
    // 获取类型标识。     /* typeid(T) -- 用于获取一个表达式或类型的运行时类型信息 (RTTI), 返回 std::type_info& */
    /* std::type_index -- 针对std::type_info对象的包装器，主要设计用来作为关联容器（如 std::map）的键。*/
    const auto type_index = std::type_index(typeid(T));
    // 如果组件已经存在，则直接返回组件指针
    if (HasComponent<T>()) {
      return GetComponent<T>();
    }
    // 如果不存在则创建组件     /* std::forward -- 用于实现完美转发。传递多个参数的时候使用...标识 */
    auto new_component = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = new_component.get();
    new_component->SetOwner(this);
    components_[type_index] = std::move(new_component);
    ptr->Init();
    LOGD(TAG, "GameObject::AddComponent: {} added component {}", name_, typeid(T).name());
    return ptr;
  }

  template <typename T>
  T* GetComponent() const {
    static_assert(std::is_base_of_v<engine::component::Component, T>, "T 必须继承自 Component");
    const auto type_index = std::type_index(typeid(T));
    if (const auto it = components_.find(type_index); it != components_.end()) {
      // 返回unique_ptr的裸指针。(肯定是T类型, static_cast其实并无必要，但保留可以使我们意图更清晰)
      return static_cast<T*>(it->second.get());
    }
    return nullptr;
  }

  template <typename T>
  bool HasComponent() const {
    static_assert(std::is_base_of_v<engine::component::Component, T>, "T 必须继承自 Component");
    // contains方法为 C++20 新增
    return components_.contains(std::type_index(typeid(T)));
  }

  template <typename T>
  void RemoveComponent() {
    static_assert(std::is_base_of_v<engine::component::Component, T>, "T 必须继承自 Component");
    const auto type_index = std::type_index(typeid(T));
    if (const auto it = components_.find(type_index); it != components_.end()) {
      it->second->Clean();
      components_.erase(it);
    }
  }

  // 关键循环函数
  void Update(float delta_time_s, engine::core::Context& context);
  void Render(engine::core::Context& context);
  void Clean();
  void HandleInput(engine::core::Context& context);

 private:
  std::string name_;
  std::string tag_;
  std::unordered_map<std::type_index, std::unique_ptr<engine::component::Component>> components_;
  bool need_remove_ = false;
};

}  // namespace engine::object
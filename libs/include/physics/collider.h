#ifndef COLLIDER_H
#define COLLIDER_H
#include <variant>

#include "math/shape.h"
#include <functional>

namespace physics
{
 class Collider
 {
 private:
  std::variant<math::Circle, math::AABB, math::Polygon> shape_ = math::Circle(0);

  float bounciness_ = 0.0f;
  float friction_ = 0.0f;
  float offset_ = 0.0f;

  bool is_trigger_ = false;

  public:
  Collider() = default;
  Collider(const std::variant<math::Circle, math::AABB, math::Polygon>& shape, const float bounciness, const float friction, const bool is_trigger)
  {
   shape_ = shape;
   bounciness_ = bounciness;
   friction_ = friction;
   is_trigger_ = is_trigger;
  };

  [[nodiscard]] std::variant<math::Circle, math::AABB, math::Polygon> shape() const { return shape_; }
  [[nodiscard]] float bounciness() const { return bounciness_; }
  [[nodiscard]] float friction() const { return friction_; }
  [[nodiscard]] bool is_trigger() const { return is_trigger_; }

  void set_shape(const std::variant<math::Circle, math::AABB, math::Polygon>& shape){ shape_ = shape; }
  void set_bounciness(const float restitution){ bounciness_ = restitution; }
  void set_friction(const float friction){ friction_ = friction; }
  void set_is_trigger(const bool is_trigger){ is_trigger_ = is_trigger; }
 };


 struct ColliderPair
 {
  Collider collider_a_;
  Collider collider_b_;

  bool operator==(const ColliderPair& other) const
  {
   return collider_a_ == other.collider_a_ && collider_b_ == other.collider_b_ ||
    collider_a_ == other.collider_b_ && collider_b_ == other.collider_a_;
  }
 };;

}
#endif //COLLIDER_H

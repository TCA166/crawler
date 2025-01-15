#include "collision.hpp"

#include "../settings.hpp"

static bool inline get_intersection(float fDst1, float fDst2, glm::vec3 P1,
                                    glm::vec3 P2, glm::vec3 &point) {
  if ((fDst1 * fDst2) >= 0.0f || (fDst1 == fDst2))
    return false;
  point = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
  return true;
}

static bool inline in_box(glm::vec3 point, glm::vec3 negbounds,
                          glm::vec3 bounds, axes axis) {
  return (axis == X && point.z > negbounds.z && point.z < bounds.z &&
          point.y > negbounds.y && point.y < bounds.y) ||
         (axis == Y && point.z > negbounds.z && point.z < bounds.z &&
          point.x > negbounds.x && point.x < bounds.x) ||
         (axis == Z && point.x > negbounds.x && point.x < bounds.x &&
          point.y > negbounds.y && point.y < bounds.y);
}

bool check_line_box(glm::vec3 negbounds, glm::vec3 bounds, glm::vec3 a,
                    glm::vec3 b, glm::vec3 &point) {
  if ((b.x < negbounds.x && a.x < negbounds.x) ||
      (b.x > bounds.x && a.x > bounds.x) ||
      (b.y < negbounds.y && a.y < negbounds.y) ||
      (b.y > bounds.y && a.y > bounds.y) ||
      (b.z < negbounds.z && a.z < negbounds.z) ||
      (b.z > bounds.z && a.z > bounds.z))
    return false;
  if (a.x > negbounds.x && a.x < bounds.x && a.y > negbounds.y &&
      a.y < bounds.y && a.z > negbounds.z && a.z < bounds.z) {
    point = a;
    return true;
  }
  if ((get_intersection(a.x - negbounds.x, b.x - negbounds.x, a, b, point) &&
       in_box(point, negbounds, bounds, X)) ||
      (get_intersection(a.y - negbounds.y, b.y - negbounds.y, a, b, point) &&
       in_box(point, negbounds, bounds, Y)) ||
      (get_intersection(a.z - negbounds.z, b.z - negbounds.z, a, b, point) &&
       in_box(point, negbounds, bounds, Z)) ||
      (get_intersection(a.x - bounds.x, b.x - bounds.x, a, b, point) &&
       in_box(point, negbounds, bounds, X)) ||
      (get_intersection(a.y - bounds.y, b.y - bounds.y, a, b, point) &&
       in_box(point, negbounds, bounds, Y)) ||
      (get_intersection(a.z - bounds.z, b.z - bounds.z, a, b, point) &&
       in_box(point, negbounds, bounds, Z)))
    return true;

  return false;
}

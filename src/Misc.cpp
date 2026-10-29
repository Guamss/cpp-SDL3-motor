#include "Misc.h"

float signed_triangle_area(float ax, float ay, float bx, float by, float cx,
                           float cy) {
  return 0.5 * ((bx - ax) * (cy - ay) - (cx - ax) * (by - ay));
}
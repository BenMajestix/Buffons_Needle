#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "buffon.h"

int on_segment(point p, point q, point r) {
  if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) &&
      q.y >= min(p.y, r.y))
    return 1;
  return 0;
}

int orientation(point p, point q, point r) {
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (val == 0)
    return 0; // collinear

  return (val > 0) ? 1 : 2; // clock or counterclock wise
}

int do_intersect(point p1, point q1, point p2, point q2) {
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);
  if (o1 != o2 && o3 != o4)
    return 1;
  // p1, q1 and p2 are collinear and p2 lies on segment p1q1
  if (o1 == 0 && on_segment(p1, p2, q1))
    return 1;
  // p1, q1 and q2 are collinear and q2 lies on segment p1q1
  if (o2 == 0 && on_segment(p1, q2, q1))
    return 1;
  // p2, q2 and p1 are collinear and p1 lies on segment p2q2
  if (o3 == 0 && on_segment(p2, p1, q2))
    return 1;
  // p2, q2 and q1 are collinear and q1 lies on segment p2q2
  if (o4 == 0 && on_segment(p2, q1, q2))
    return 1;
  return 0; // Doesn't fall in any of the above cases
}


#endif

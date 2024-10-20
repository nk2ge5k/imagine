#include "delaunay.h"

#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>

#include "stb_ds.h"

typedef struct {
  Vector2 min;
  Vector2 max;

} BBox;

local Vector2 bboxCenter(BBox box) {
  Vector2 center = {
    .x = (box.min.x + box.max.x) / 2.0f,
    .y = (box.min.y + box.max.y) / 2.0f,
  };
  return center;
}

local BBox bounds(Vector2* points, i32 length) {
  BBox box = { 0 };
  if (length == 0) {
    return box;
  }

  box.min = points[0];
  box.max = points[0];

  Vector2* point;
  for (i32 i = 1; i < length; i++) {
    point = points + i;

    if (point->x > box.max.x) {
      box.max.x = point->x;
    } else if (point->x < box.min.x) {
      box.max.x = point->x;
    }

    if (point->y > box.max.y) {
      box.max.y = point->y;
    } else if (point->y < box.min.y) {
      box.max.y = point->y;
    }
  }

  return box;
}

local f32 triangleArea(Vector2 a, Vector2 b, Vector2 c) {
  return (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
}

local Vector2 triangleCircumcircleCenter(Vector2 a, Vector2 b, Vector2 c) {
  f32 dx = b.x - a.x;
  f32 dy = b.y - a.y;
  f32 ex = c.x - a.x;
  f32 ey = c.y - a.y;

  f32 bl = dx * dx + dy * dy;
  f32 cl = ex * ex + ey * ey;
  f32 d  = 0.5 / (dx * ey - dy * ex);

  Vector2 center = {
    .x = a.x + (ey * bl - dy * cl) * d,
    .y = a.y + (dx * cl - ex * bl) * d,
  };

  return center;
}

local f32 triangleCircumcircleRadius(Vector2 a, Vector2 b, Vector2 c) {
  f32 dx = b.x - a.x;
  f32 dy = b.y - a.y;
  f32 ex = c.x - a.x;
  f32 ey = c.y - a.y;

  f32 bl = dx * dx + dy * dy;
  f32 cl = ex * ex + ey * ey;
  f32 d  = 0.5 / (dx * ey - dy * ex);

  f32 x = (ey * bl - dy * cl) * d;
  f32 y = (dx * cl - ex * bl) * d;

  return x * x + y * y;
}

local i32 closestPoint(Vector2 to, Vector2* points, i32 length, i32 exclude) {
  f32 distance;
  f32 cur_distance;

  i32 first    = 0;
  i32 closest  = -1;
  for (; first < length; first++) {
    if (first != exclude) {
      distance = Vector2DistanceSqr(to, points[0]);
      closest  = first;
      break;
    }
  }

  for (i32 i = 0; i < length; i++) {
    if (i == closest || i == exclude) continue;
    cur_distance = Vector2DistanceSqr(to, points[i]);
    if (cur_distance < distance) {
      closest  = i;
      distance = cur_distance;
    }
  }

  return closest;
}

local i32 formsSmallestCircle(i32 ai, i32 bi, Vector2* points, i32 length) {

  f32 radius;
  f32 cur_radius;

  i32 first    = 0;
  i32 closest  = -1;
  for (; first < length; first++) {
    if (first != ai && first != bi) {
      radius  = triangleCircumcircleRadius(points[ai], points[bi], points[first]);
      closest = first;
      break;
    }
  }

  for (i32 i = 0; i < length; i++) {
    if (i == ai || i == bi || i == first) continue;

    cur_radius = triangleCircumcircleRadius(points[ai], points[bi], points[closest]);
    if (cur_radius < radius) {
      closest = i;
      radius  = cur_radius;
    }
  }

  return closest;
}

// @todo: Original implementations precomputes all of the distances before
// the sort, which may be a faster way but at the time of writing this comment
// I was too lazy and stupid to think of the similar implementation.
// Instead of points this function should operate with ids (indices).
local i32 compareDistances(void* ctx, const void* lhs, const void* rhs) {
  Vector2* center = CAST(Vector2*, ctx);
  Vector2* a      = CAST(Vector2*, lhs);
  Vector2* b      = CAST(Vector2*, rhs);

  f32 center_a = Vector2DistanceSqr(*center, *a);
  f32 center_b = Vector2DistanceSqr(*center, *b);

  return (center_a > center_b) ? 1 : -1;
}

local void sortByDistance(Vector2 from, Vector2* points, i32 length) {
  qsort_r(points, length, sizeof(Vector2), &from, compareDistances);
}


void delaunay(Vector2* points, i32 length) {
  if (length < 3) {
    return;
  }

  BBox box           = bounds(points, length);
  Vector2 box_center = bboxCenter(box);

  // pick a seed point close to the center
  i32 ai = closestPoint(box_center, points, length, -1);
  i32 bi = closestPoint(points[ai], points, length, ai);
  i32 ci = formsSmallestCircle(ai, bi, points, length);


  // Orient triangle counter-clocwise
  if (triangleArea(points[ai], points[bi], points[ci]) < 0) {
    i32 tmp = bi;
    bi = ci;
    ci = tmp;
  }

  Vector2 center = triangleCircumcircleCenter(points[ai], points[bi], points[ci]);

  DrawLineEx(points[ai], points[bi], 2, RED);
  DrawLineEx(points[bi], points[ci], 2, GREEN);
  DrawLineEx(points[ci], points[ai], 2, BLUE);

  f32 radius_sqr = Vector2Distance(center, points[bi]);

  DrawCircleLinesV(center, radius_sqr, BLACK);

  sortByDistance(center, points, length);
}

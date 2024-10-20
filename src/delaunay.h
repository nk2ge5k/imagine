#ifndef DELAYNAY_H
#define DELAYNAY_H

#include "types.h"

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

void delaunay(Vector2* points, i32 length);

#ifdef __cplusplus
}
#endif

#endif // DELAYNAY_H

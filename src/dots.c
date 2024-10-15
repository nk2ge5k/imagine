#include <raylib.h>
#include <raymath.h>

#include "types.h"

typedef enum {
  FIGURE_CIRCLE   = 0,
  FIGURE_SQUARE   = 1,
  FIGURE_TRIANGLE = 2,
  FIGURE_STAR     = 3,
  FIGURE_RHOMBUS  = 4,
} Figure;

#define RADS(degs) (degs * M_PI_180)

local void renderCircle(Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
    .x = area.x + area.width / 2.0f,
    .y = area.y + area.width / 2.0f,
  };

  DrawCircleV(center, lum * radius, color);
}

local void renderSquare(Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
    .x = area.x + area.width / 2.0f,
    .y = area.y + area.width / 2.0f,
  };

  // f32 size = (radius * lum) * 0.5;
  f32 size = (radius * lum);
  Vector2 strip[5];

  strip[0] = (Vector2){
    .x = center.x + size * cos(RADS(-45.0f)),
    .y = center.y + size * sin(RADS(-45.0f)),
  };
  strip[1] = (Vector2){
    .x = center.x + size * cos(RADS(-135.0f)),
    .y = center.y + size * sin(RADS(-135.0f)),
  };
  strip[2] = (Vector2){
    .x = center.x + size * cos(RADS(-225.0f)),
    .y = center.y + size * sin(RADS(-225.0f)),
  };
  strip[3] = (Vector2){
    .x = center.x + size * cos(RADS(-315.0f)),
    .y = center.y + size * sin(RADS(-315.0f)),
  };
  strip[4] = (Vector2){
    .x = center.x + size * cos(RADS(-45.0f)),
    .y = center.y + size * sin(RADS(-45.0f)),
  };

  DrawTriangleStrip(strip, 5, color);
}

local void renderTriangle(Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
      .x = area.x + area.width / 2.0f,
      .y = area.y + area.width / 2.0f,
  };

  // f32 size = (radius * lum) * 0.5;
  f32 size = (radius * lum);

  Vector2 a = (Vector2){
    .x = center.x + size * cos(RADS(-90.0f)),
    .y = center.y + size * sin(RADS(-90.0f)),
  };
  Vector2 b = (Vector2){
    .x = center.x + size * cos(RADS(-210.0f)),
    .y = center.y + size * sin(RADS(-210.0f)),
  };
  Vector2 c = (Vector2){
    .x = center.x + size * cos(RADS(-330.0f)),
    .y = center.y + size * sin(RADS(-330.0f)),
  };

  DrawTriangle(a, b, c, color);
}

local void renderStar(Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
    .x = area.x + area.width / 2.0f,
    .y = area.y + area.width / 2.0f,
  };

  f32 outer_radius = radius * lum;
  f32 inner_radius = outer_radius * 0.5f;
  f32 step         = RADS(-36.0f);
  f32 angle        = RADS(-90.0f) - step;

  i32 cur = 0;
  Vector2 strip[21];
  strip[cur++] = center;

  for (i32 i = 0; i < 5; i++) {
    {
      strip[cur++] = (Vector2){
        .x = center.x + inner_radius * cos(angle),
        .y = center.y + inner_radius * sin(angle),
      };

      angle += step;

      strip[cur++] = (Vector2){
        .x = center.x + outer_radius * cos(angle),
        .y = center.y + outer_radius * sin(angle),
      };
    }
    {
      strip[cur++] = (Vector2){
        .x = center.x + outer_radius * cos(angle),
        .y = center.y + outer_radius * sin(angle),
      };

      angle += step;

      strip[cur++] = (Vector2){
        .x = center.x + inner_radius * cos(angle),
        .y = center.y + inner_radius * sin(angle),
      };
    }
  }

  DrawTriangleFan(strip, cur, color);
}

local void renderRhombus(Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
    .x = area.x + area.width / 2.0f,
    .y = area.y + area.width / 2.0f,
  };

  f32 size = radius * lum;
  Vector2 strip[5];

  strip[0] = (Vector2){
    .x = center.x + size * cos(RADS(0.0f)),
    .y = center.y + size * sin(RADS(0.0f)),
  };
  strip[1] = (Vector2){
    .x = center.x + size * cos(RADS(-90.0f)),
    .y = center.y + size * sin(RADS(-90.0f)),
  };
  strip[2] = (Vector2){
    .x = center.x + size * cos(RADS(-180.0f)),
    .y = center.y + size * sin(RADS(-180.0f)),
  };
  strip[3] = (Vector2){
    .x = center.x + size * cos(RADS(-270.0f)),
    .y = center.y + size * sin(RADS(-270.0f)),
  };
  strip[4] = (Vector2){
    .x = center.x + size * cos(RADS(0)),
    .y = center.y + size * sin(RADS(0)),
  };

  DrawTriangleStrip(strip, 5, color);
}

local void renderFigureButton(Rectangle rect, Figure* figure) {
  const f32 padding = 10.0f;

  {
    bool is_mouse_over = CheckCollisionPointRec(GetMousePosition(), rect);
    if (is_mouse_over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      *figure = (*figure >= FIGURE_RHOMBUS) ? FIGURE_CIRCLE : *figure + 1;
    }

    DrawRectangleRec(rect, is_mouse_over ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(rect, 1.0f, is_mouse_over ? GRAY : BLACK);
  }

  f32 size = ((rect.width < rect.height) ? rect.width : rect.height) - padding;

  switch (*figure) {
  case FIGURE_CIRCLE:
    renderCircle(rect, RED, 0.5f, size);
    break;
  case FIGURE_SQUARE:
    renderSquare(rect, DARKBLUE, 0.5f, size);
    break;
  case FIGURE_TRIANGLE:
    renderTriangle(rect, DARKGREEN, 0.5f, size);
    break;
  case FIGURE_STAR:
    renderStar(rect, ORANGE, 0.5f, size);
    break;
  case FIGURE_RHOMBUS:
    renderRhombus(rect, VIOLET, 0.5f, size);
    break;
  }
}

local void renderFigure(Rectangle area, Color color, f32 lum, f32 radius, Figure figure) {
  if (lum == 0) {
    return;
  }

  switch (figure) {
    case FIGURE_CIRCLE:
      renderCircle(area, color, lum, radius);
      break;
    case FIGURE_SQUARE:
      renderSquare(area, color, lum, radius);
      break;
    case FIGURE_TRIANGLE:
      renderTriangle(area, color, lum, radius);
      break;
    case FIGURE_STAR:
      renderStar(area, color, lum, radius);
      break;
    case FIGURE_RHOMBUS:
      renderRhombus(area, color, lum, radius);
      break;
  }
}

local bool loadDroppedImage(Image* image) {
  if (!IsFileDropped()) {
    return false;
  }

  bool loaded = false;

  FilePathList files = LoadDroppedFiles();
  for (u32 i = 0; i < files.count; i++) {
    Image img = LoadImage(files.paths[i]);

    if (IsImageReady(img)) {
      UnloadImage(*image);
      *image = img;
      loaded = true;

      break;
    }
  }

  UnloadDroppedFiles(files);
  return loaded;
}

local Color averageColor(Image img, Rectangle area) {
  u64 r = 0;
  u64 g = 0;
  u64 b = 0;

  i32 xend = area.x + area.width;
  i32 yend = area.y + area.height;

  xend = (xend >= img.width) ? img.width : xend;
  yend = (yend >= img.height) ? img.height : yend;

  for (i32 y = CAST(i32, area.y); y < yend; y++) {
    for (i32 x = CAST(i32, area.x); x < xend; x++) {
      Color color = GetImageColor(img, x, y);
      r += color.r;
      g += color.g;
      b += color.b;
    }
  }

  i32 count = area.width * area.height;

  Color result = {
      .r = CAST(u8, r / count),
      .g = CAST(u8, g / count),
      .b = CAST(u8, b / count),
      .a = 255,
  };

  return result;
}

i32 main(void) {
  InitWindow(1024, 768, "dots");

  Image image = { 0 };
  Figure figure = FIGURE_CIRCLE;

  Camera2D camera = { 0 };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  camera.offset = (Vector2){ 
    .x = GetScreenWidth() / 2.0f,
    .y = GetScreenHeight() / 2.0f,
  };

  while (!WindowShouldClose()) {
    if (loadDroppedImage(&image)) {
      camera.zoom   = 1.0f;
      camera.target = (Vector2){
        .x = image.width / 2.0f,
        .y = image.height / 2.0f,
      };
    }

    Rectangle figure_button = {
      .x      = GetScreenWidth() - 45,
      .y      = 5,
      .width  = 40,
      .height = 40,
    };

    BeginDrawing();
    ClearBackground(WHITE);

    i32 width = GetScreenWidth();
    i32 height = GetScreenHeight();

    Vector2 mouse = GetMousePosition();
    f32 coefy  = mouse.y / height;
    f32 coefx  = mouse.x / width;
    i32 step   = Clamp(50 * coefy, 1, 50);
    f32 radius = Clamp((step / 2.0f) * coefx, 1, 25);

    step = step % 2 == 0 ? step + 1 : step;

    camera.zoom += GetMouseWheelMove() * 0.05f;
    camera.zoom = Clamp(camera.zoom, 0.1f, 3.0f);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= 2.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += 2.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= 2.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += 2.0f;

    BeginMode2D(camera);
    if (IsImageReady(image)) {
      for (i32 y = 0; y < image.height; y += step) {
        i32 x = (y % 2 == 0) ? 0 : step / 2;
        for (; x < image.width; x += step) {
          Rectangle area = {
              .x      = x,
              .y      = y,
              .width  = step,
              .height = step,
          };

          Color avg = averageColor(image, area);

          f32 rf = (255.0f - avg.r);
          f32 gf = (255.0f - avg.g);
          f32 bf = (255.0f - avg.b);
          f32 lum = Clamp(sqrt(rf * rf * .299f + gf * gf * .587f + bf * bf * .114f) / 255.0f, 0.0f, 1.0f);

          renderFigure(area, avg, lum, radius, figure);
        }
      }
    }
    EndMode2D();

    renderFigureButton(figure_button, &figure);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

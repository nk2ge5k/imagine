// Copyright 2024, Geogii Chernukhin <nk2ge5k@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>

#include "types.h"

#define MAX_FILENAME_SIZE 1024
#define RADS(degs) (degs * M_PI_180)

////////////////////////////////////////////////////////////////////////////////
/// FIGURES
////////////////////////////////////////////////////////////////////////////////

typedef enum {
  FIGURE_CIRCLE   = 0,
  FIGURE_SQUARE   = 1,
  FIGURE_TRIANGLE = 2,
  FIGURE_STAR     = 3,
  FIGURE_RHOMBUS  = 4,

  _FIGURE_MAX
} Figure;

typedef void DrawCicrcleFn(Vector2 center, f32 radius, Color color);
typedef void DrawTriangleStripFn(const Vector2 *points, i32 pointCount, Color color);
typedef void DrawTriangleFn(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
typedef void DrawTriangleFanFn(const Vector2 *points, i32 pointCount, Color color);

typedef struct {
  DrawCicrcleFn* draw_circle;
  DrawTriangleFn* draw_triangle;
  DrawTriangleFanFn* draw_triangle_fan;
  DrawTriangleStripFn* draw_triangle_strip;

} Renderer;

local void renderCircle(Renderer render, Rectangle area, Color color, f32 lum, f32 radius) {
  Vector2 center = {
    .x = area.x + area.width / 2.0f,
    .y = area.y + area.width / 2.0f,
  };
  render.draw_circle(center, lum * radius, color);
}

local void renderSquare(Renderer render, Rectangle area, Color color, f32 lum, f32 radius) {
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

  render.draw_triangle_strip(strip, 5, color);
}

local void renderTriangle(Renderer render, Rectangle area, Color color, f32 lum, f32 radius) {
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

  render.draw_triangle(a, b, c, color);
}

local void renderStar(Renderer render, Rectangle area, Color color, f32 lum, f32 radius) {
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

  render.draw_triangle_fan(strip, cur, color);
}

local void renderRhombus(Renderer render, Rectangle area, Color color, f32 lum, f32 radius) {
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

  render.draw_triangle_strip(strip, 5, color);
}


////////////////////////////////////////////////////////////////////////////////
/// CONTROLS
////////////////////////////////////////////////////////////////////////////////

typedef struct {
  bool is_mouse_over;
  bool is_clicked;
} Button;

local void updateButton(Button* state, Rectangle rect) {
  Vector2 mouse  = GetMousePosition();

  if (CheckCollisionPointRec(mouse, rect)) {
    state->is_mouse_over = true;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      state->is_clicked = !state->is_clicked;
    }
  } else {
    state->is_mouse_over = false;
  }
}

local void renderButton(Button* state, Rectangle rect) {
  DrawRectangleRec(rect, state->is_mouse_over ? LIGHTGRAY : WHITE);
  DrawRectangleLinesEx(rect, 2, state->is_mouse_over ? BLACK : GRAY);
}

/// FIGURE CONTROLS ////////////////////////////////////////////////////////////

typedef struct {
  // Selected figure
  Figure figure;
  // Button is highlighted
  bool highlighted;
} FigureButtonState;

local Rectangle rectFigureButton(void) {
  Rectangle rect = {
    .x      = GetScreenWidth() - 45,
    .y      = 5,
    .width  = 40,
    .height = 40,
  };
  return rect;
}

local void updateFigureButton(FigureButtonState* state) {
  Rectangle rect = rectFigureButton();
  Vector2 mouse  = GetMousePosition();

  if (CheckCollisionPointRec(mouse, rect)) {
    state->highlighted = true;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      state->figure++;
      if (state->figure >= _FIGURE_MAX) state->figure = 0;
    }
  } else {
    state->highlighted = false;
  }
}

local void renderFigureButton(Renderer render, FigureButtonState* state) {
  local const f32 padding = 10.0f;

  Rectangle rect = rectFigureButton();
  DrawRectangleRec(rect, state->highlighted ? LIGHTGRAY : WHITE);
  DrawRectangleLinesEx(rect, 2, state->highlighted ? BLACK : GRAY);

  f32 size = ((rect.width < rect.height) ? rect.width : rect.height) - padding;

  switch (state->figure) {
  case FIGURE_CIRCLE:
    renderCircle(render, rect, RED, 0.5f, size);
    break;
  case FIGURE_SQUARE:
    renderSquare(render, rect, DARKBLUE, 0.5f, size);
    break;
  case FIGURE_TRIANGLE:
    renderTriangle(render, rect, DARKGREEN, 0.5f, size);
    break;
  case FIGURE_STAR:
    renderStar(render, rect, ORANGE, 0.5f, size);
    break;
  case FIGURE_RHOMBUS:
    renderRhombus(render, rect, VIOLET, 0.5f, size);
    break;
  default:
    break;
  }
}

/// STEP AND RADIUS CONTROLS ///////////////////////////////////////////////////

typedef struct {
  bool highlighted;

  f32 x;
  f32 y;

  i32 step;
  i32 radius;
} StepRadiusState;

local Rectangle rectStepRadius(StepRadiusState* state) {
  local const f32 margin    = 10.0f;
  local const f32 size      = 100.0f;
  local const f32 full_size = 200.0f;

  i32 height = GetScreenHeight();
  Rectangle rect;

  if (!state->highlighted) {
    rect.x      = margin;
    rect.y      = height - size - margin;
    rect.width  = size;
    rect.height = size;
  } else {
    rect.x      = margin;
    rect.y      = height - full_size - margin;
    rect.width  = full_size;
    rect.height = full_size;
  }

  return rect;
}

local void updateStepRadius(StepRadiusState* state) {
  local const f32 max_step  = 50;

  Rectangle rect = rectStepRadius(state);
  Vector2 mouse  = GetMousePosition();

  f32 radius = rect.width * 0.07;
  f32 xmin   = rect.x + radius;
  f32 xmax   = rect.x + rect.width - radius;
  f32 ymin   = rect.y + radius;
  f32 ymax   = rect.y + rect.height - radius;


  if (CheckCollisionPointRec(mouse, rect)) {
    state->highlighted = true;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      state->x = Remap(Clamp(mouse.x, xmin, xmax), xmin, xmax, 0.0f, 1.0f);
      state->y = Remap(Clamp(mouse.y, ymin, ymax), ymin, ymax, 0.0f, 1.0f);
    }
  } else {
    state->highlighted = false;
  }

  state->step   = Clamp(max_step * state->y, 4, max_step);
  state->step   = state->step % 2 == 0 ? state->step + 1 : state->step;
  state->radius = (state->step / 2.0f) * state->x;
}

local void renderStepRadiusControl(StepRadiusState* state) {
  local const Color rect_color    = { 200, 200, 200, 55 };
  local const Color rect_hi_color = { 200, 200, 200, 125 };
  local const Color dot_color     = { 230, 41, 55, 55 };
  local const Color dot_hi_color  = { 230, 41, 55, 175 };

  Rectangle rect = rectStepRadius(state);
  DrawRectangleRec(rect, state->highlighted ? rect_hi_color : rect_color);
  DrawRectangleLinesEx(rect, 1, BLACK);

  f32 radius = rect.width * 0.07;
  f32 xmin   = rect.x + radius;
  f32 xmax   = rect.x + rect.width - radius;
  f32 ymin   = rect.y + radius;
  f32 ymax   = rect.y + rect.height - radius;

  Vector2 center = {
    .x = xmin + ((xmax - xmin) * state->x),
    .y = ymin + ((ymax - ymin) * state->y),
  };

  DrawCircleV(center, radius, state->highlighted ? dot_hi_color : dot_color);
}

/// BLACK AND WHITE CONTROLS ///////////////////////////////////////////////////

local Rectangle rectBWButton(void) {
  Rectangle rect = {
    .x      = GetScreenWidth() - 45,
    .y      = 50,
    .width  = 40,
    .height = 40,
  };
  return rect;
}

local void updateBWButton(Button* state) {
  Rectangle rect = rectBWButton();
  updateButton(state, rect);
}

local void renderBW(Rectangle rect) {
  Rectangle black = {
    .x      = rect.x,
    .y      = rect.y,
    .width  = rect.width / 2,
    .height = rect.height,
  };
  Rectangle white = {
    .x      = rect.x + rect.width / 2,
    .y      = rect.y,
    .width  = rect.width / 2,
    .height = rect.height,
  };

  DrawRectangleRec(black, BLACK);
  DrawRectangleRec(white, WHITE);
}

local void renderRainbow(Rectangle rect) {
  static Color colors[] = { RED, ORANGE, YELLOW, GREEN, SKYBLUE, BLUE, VIOLET };
  static i32 ncolors    = sizeof(colors) / sizeof(Color);

  f32 width = rect.width / ncolors;
  for (i32 i = 0; i < ncolors; i++) {
    Rectangle strip = {
      .x      = rect.x + (width * i),
      .y      = rect.y,
      .width  = width,
      .height = rect.height,
    };
    DrawRectangleRec(strip, colors[i]);
  }
}

local void renderBWButton(Button* state) {
  local const f32 padding = 10.0f;

  Rectangle rect = rectBWButton();
  renderButton(state, rect);

  f32 size = ((rect.width < rect.height) ? rect.width : rect.height) - padding;

  Rectangle icon = {
    .x      = rect.x + (padding / 2.0f),
    .y      = rect.y + (padding / 2.0f),
    .width  = size,
    .height = size,
  };

  if (state->is_clicked) {
    renderBW(icon);
  } else {
    renderRainbow(icon);
  }
}

/// LUM CONTROLS ///////////////////////////////////////////////////////////////

local Rectangle rectLumButton(void) {
  Rectangle rect = {
    .x      = GetScreenWidth() - 45,
    .y      = 95,
    .width  = 40,
    .height = 40,
  };
  return rect;
}

local void updateLumButton(Button* state) {
  Rectangle rect = rectLumButton();
  updateButton(state, rect);
}

local void renderLumButton(Button* state) {
  local const f32 padding = 10.0f;

  Rectangle rect = rectLumButton();
  renderButton(state, rect);

  f32 size = ((rect.width < rect.height) ? rect.width : rect.height) - padding;

  Rectangle icon = {
    .x      = rect.x + (padding / 2.0f),
    .y      = rect.y + (padding / 2.0f),
    .width  = size,
    .height = size,
  };

  if (state->is_clicked) {
    Vector2 start = {
      .x = rect.x + padding * 0.5f,
      .y = rect.y + padding * 0.5f + size,
    };

    Vector2 end = {
      .x = rect.x + padding * 0.5f + size,
      .y = rect.y + padding * 0.5f,
    };

    DrawLineEx(start, end, 2, BLACK);
  } else {
    Vector2 start = {
      .x = rect.x + padding * 0.5f,
      .y = rect.y + padding * 0.5f + size * 0.5f,
    };

    Vector2 end = {
      .x = rect.x + padding * 0.5f + size,
      .y = rect.y + padding * 0.5f + size * 0.5f,
    };

    DrawLineEx(start, end, 2, BLACK);
  }
}

/// SHIFT CONTROLS /////////////////////////////////////////////////////////////

local Rectangle rectShiftButton(void) {
  Rectangle rect = {
    .x      = GetScreenWidth() - 45,
    .y      = 140,
    .width  = 40,
    .height = 40,
  };
  return rect;
}

local void updateShiftButton(Button* state) {
  Rectangle rect = rectShiftButton();
  updateButton(state, rect);
}

local void renderShiftButton(Button* state, Figure fugure) {
  local const f32 padding = 10.0f;

  Rectangle rect = rectShiftButton();
  renderButton(state, rect);

  f32 size = ((rect.width < rect.height) ? rect.width : rect.height) - padding;

  f32 xmin = rect.x + padding * 0.5f;
  f32 ymin = rect.y + padding * 0.5f;

  if (state->is_clicked) {
    DrawCircleV((Vector2){ 
      .x = xmin + size * 0.75f,
      .y = ymin + size * 0.25f,
    }, size * 0.2f, BLACK);
    DrawCircleV((Vector2){ 
      .x = xmin + size * 0.25f,
      .y = ymin + size * 0.75f,
    }, size * 0.2f, BLACK);
  } else {
    DrawCircleV((Vector2){ 
      .x = xmin + size * 0.5f,
      .y = ymin + size * 0.25f,
    }, size * 0.2f, BLACK);
    DrawCircleV((Vector2){ 
      .x = xmin + size * 0.5f,
      .y = ymin + size * 0.75f,
    }, size * 0.2f, BLACK);
  }
}

/// SAVE CONTROLS //////////////////////////////////////////////////////////////

local Rectangle rectSaveButton(void) {
  Rectangle rect = {
    .x      = GetScreenWidth() - 45,
    .y      = 185,
    .width  = 40,
    .height = 40,
  };
  return rect;
}

local void updateSaveButton(Button* state) {
  Rectangle rect = rectSaveButton();
  updateButton(state, rect);
}

local void renderSaveButton(Button* state) {
  Rectangle rect = rectSaveButton();
  renderButton(state, rect);
  DrawText("SAVE", rect.x + 4, rect.y + rect.height / 2 - 6, 12, BLACK);
}


////////////////////////////////////////////////////////////////////////////////
/// SVG
////////////////////////////////////////////////////////////////////////////////

FILE* svg;

local void svgBegin(i32 width, i32 height, f32 radius) {
  fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
  fprintf(svg, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" " );
  fprintf(svg, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n" );
  fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" "
    "width=\"%f\" "
    "height=\"%f\" "
    "viewBox=\"%f %f %f %f\">\n",
    width + (radius * 2), height + (radius * 2),
    radius, radius, width+radius, height+radius);
}

local void svgDrawCircleV(Vector2 center, f32 radius, Color color) {
  fprintf(svg,
    "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%02x%02x%02x\"/>\n",
    center.x, center.y, radius, color.r, color.g, color.b);
}

local void svgDrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
  fprintf(svg,
    "<polygon points=\"%f,%f %f,%f %f,%f\" fill=\"#%02x%02x%02x\"/>\n",
    v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, color.r, color.g, color.b);
}

local void svgDrawTriangleFan(const Vector2 *points, i32 pointCount, Color color) {
  if (pointCount >= 3) {
    fprintf(svg, "<polygon points=\"");
    for (i32 i = 1; i < pointCount; i++) {
      if (i > 0) fprintf(svg, " ");
      fprintf(svg, "%f,%f", points[i].x, points[i].y);
    }
    fprintf(svg, "\" fill=\"#%02x%02x%02x\"/>\n", color.r, color.g, color.b);
  }
}

local void svgDrawTriangleStrip(const Vector2 *points, i32 pointCount, Color color) {
  if (pointCount >= 3) {
    fprintf(svg, "<polygon points=\"");
    for (i32 i = 0; i < pointCount; i++) {
      if (i > 0) fprintf(svg, " ");
      fprintf(svg, "%f,%f", points[i].x, points[i].y);
    }
    fprintf(svg, "\" fill=\"#%02x%02x%02x\"/>\n", color.r, color.g, color.b);
  }
}

local void svgEnd() {
  fprintf(svg, "</svg>");
}

////////////////////////////////////////////////////////////////////////////////
/// MAIN
////////////////////////////////////////////////////////////////////////////////

local bool loadDroppedImage(Image* image, char* filename) {
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

      memset(filename, 0, MAX_FILENAME_SIZE * sizeof(char));
      strncpy(filename, GetFileNameWithoutExt(files.paths[i]), MAX_FILENAME_SIZE - 1);

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

local void renderFigure(Renderer render, Rectangle area, Color color, f32 lum, f32 radius, Figure figure) {
  if (lum == 0) {
    return;
  }

  switch (figure) {
    case FIGURE_CIRCLE:
      renderCircle(render, area, color, lum, radius);
      break;
    case FIGURE_SQUARE:
      renderSquare(render, area, color, lum, radius);
      break;
    case FIGURE_TRIANGLE:
      renderTriangle(render, area, color, lum, radius);
      break;
    case FIGURE_STAR:
      renderStar(render, area, color, lum, radius);
      break;
    case FIGURE_RHOMBUS:
      renderRhombus(render, area, color, lum, radius);
      break;
    default:
      break;
  }
}

local void renderImage(Renderer render, Image image,
    Figure figure, i32 step, f32 radius, bool shift, bool bw, bool size_lum) {
  for (i32 y = 0; y < image.height; y += step) {
    i32 x = (shift && (y % 2 == 0)) ? 0 : step / 2;
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

      Color color = avg;
      if (bw) {
        color.r = 255.0f * (1.0f - lum);
        color.g = 255.0f * (1.0f - lum);
        color.b = 255.0f * (1.0f - lum);
      }

      f32 mul = size_lum ? lum : 1.0f;
      renderFigure(render, area, color, mul, radius, figure);
    }
  }
}

i32 main(void) {
  static const char text[] = "Drag and drop your image here";
  static const char subtext[] = "supported file formats: .png, .jpg";

  char filename[MAX_FILENAME_SIZE] = { 0 };

  InitWindow(1024, 768, "dots");
  SetTargetFPS(30);

  i32 text_width = MeasureText(text, 30);
  i32 subtext_width = MeasureText(subtext, 24);

  Image image                       = { 0 };
  StepRadiusState step_radius_state = { false, 0.5, 0.5 };
  FigureButtonState figure_state    = { 0 };
  Button bw_state                   = { 0 };
  Button lum_state                  = { 0 };
  Button shift_state                = { 0 };
  Button save_state                 = { 0 };

  Renderer ray_renderer = {
    .draw_circle          = DrawCircleV,
    .draw_triangle        = DrawTriangle,
    .draw_triangle_fan    = DrawTriangleFan,
    .draw_triangle_strip  = DrawTriangleStrip,
  };
  Renderer svg_renderer = {
    .draw_circle          = svgDrawCircleV,
    .draw_triangle        = svgDrawTriangle,
    .draw_triangle_fan    = svgDrawTriangleFan,
    .draw_triangle_strip  = svgDrawTriangleStrip,
  };

  Camera2D camera = { 0 };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  camera.offset = (Vector2){ 
    .x = GetScreenWidth() / 2.0f,
    .y = GetScreenHeight() / 2.0f,
  };

  while (!WindowShouldClose()) {
    if (loadDroppedImage(&image, filename)) {
      camera.zoom   = 1.0f;
      camera.target = (Vector2){
        .x = image.width / 2.0f,
        .y = image.height / 2.0f,
      };
    }

    i32 width = GetScreenWidth();
    i32 height = GetScreenHeight();

    updateStepRadius(&step_radius_state);
    updateFigureButton(&figure_state);
    updateBWButton(&bw_state);
    updateLumButton(&lum_state);
    updateShiftButton(&shift_state);
    updateSaveButton(&save_state);

    if (save_state.is_clicked) {
      if (IsImageReady(image)) {
        const char* filepath = TextFormat("%s/Desktop/%s.svg",
            getenv("HOME"), filename);
        if (FileExists(filepath)) {
          for (i32 i = 1; i < 99; i++) {
            filepath = TextFormat("%s/Desktop/%s_%00d.svg",
                getenv("HOME"), filename, i);
            if (!FileExists(filepath)) {
              svg = fopen(filepath, "w");
              break;
            }
          }
        } else {
          svg = fopen(filepath, "w");
        }

        if (svg != NULL) {
          svgBegin(image.width, image.height, step_radius_state.radius);
          renderImage(svg_renderer, image,
              figure_state.figure,
              step_radius_state.step,
              step_radius_state.radius,
              shift_state.is_clicked,
              bw_state.is_clicked,
              lum_state.is_clicked);
          svgEnd();
          fclose(svg);
          svg = NULL;
        } else {
          fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
        }
      }
      save_state.is_clicked = false;
    }

    camera.zoom += GetMouseWheelMove() * 0.05f;
    camera.zoom = Clamp(camera.zoom, 0.1f, 3.0f);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= 2.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += 2.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= 2.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += 2.0f;

    BeginDrawing();
    ClearBackground(WHITE);

    if (IsImageReady(image)) {
      BeginMode2D(camera);
      renderImage(ray_renderer, image,
          figure_state.figure,
          step_radius_state.step,
          step_radius_state.radius,
          shift_state.is_clicked,
          bw_state.is_clicked,
          lum_state.is_clicked);
      EndMode2D();
    } else {
      i32 y = height / 2 - 40;

      DrawText(text, (width / 2) - (text_width / 2), y, 30, BLACK);
      DrawText(subtext, (width / 2) - (subtext_width / 2), y + 30, 24, GRAY);
    }

    renderStepRadiusControl(&step_radius_state);
    renderFigureButton(ray_renderer, &figure_state);
    renderBWButton(&bw_state);
    renderLumButton(&lum_state);
    renderShiftButton(&shift_state, figure_state.figure);
    renderSaveButton(&save_state);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

#include "draw_graphics.h"

#include <stdio.h>
#include <stdlib.h>

// Global color constants
constexpr unsigned int COLOR_WHITE = 0xFFFFFFFF;
constexpr unsigned int COLOR_BLACK = 0x000000FF;
constexpr unsigned int COLOR_RED = 0x000000FF;
constexpr unsigned int COLOR_GREEN = 0x00FF00FF;
constexpr unsigned int COLOR_BLUE = 0xFF0000FF;
constexpr unsigned int COLOR_YELLOW = 0x00FFFF00;
constexpr unsigned int COLOR_PURPLE = 0xFF00FFFF;
constexpr unsigned int COLOR_CYAN = 0xFFFFFF00;

// Helper function to get pixel pointer
static inline int* getPixelPtr(ScreenBufferInfo buffer_info, int x, int y) {
  int bytes_per_pixel = 4;  // RGBA8888
  return buffer_info.buffer_ptr + (y * buffer_info.buffer_stride / bytes_per_pixel) + x;
}

// Helper function to check if pixel is within bounds
static inline bool isPixelInBounds(ScreenBufferInfo buffer_info, int x, int y) {
  return x >= 0 && x < buffer_info.buffer_size[0] && y >= 0 && y < buffer_info.buffer_size[1];
}

/**
 * @brief Draw a filled rectangle
 * @param buffer_info Structure containing buffer information
 * @param x X coordinate of the rectangle's top-left corner
 * @param y Y coordinate of the rectangle's top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle in RGBA8888 format
 */
void drawRectangle(ScreenBufferInfo buffer_info, int x, int y, int width, int height,
                   unsigned int color) {
  if (width <= 0 || height <= 0) {
    return;
  }

  for (int dy = 0; dy < height; dy++) {
    int draw_y = y + dy;
    if (draw_y >= buffer_info.buffer_size[1] || draw_y < 0) {
      continue;
    }

    for (int dx = 0; dx < width; dx++) {
      int draw_x = x + dx;
      if (draw_x >= buffer_info.buffer_size[0] || draw_x < 0) {
        continue;
      }

      *getPixelPtr(buffer_info, draw_x, draw_y) = color;
    }
  }
}

/**
 * @brief Draw a rectangle with checkerboard pattern
 * @param buffer_info Structure containing buffer information
 * @param x X coordinate of the rectangle's top-left corner
 * @param y Y coordinate of the rectangle's top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color1 First color for checkerboard pattern
 * @param color2 Second color for checkerboard pattern
 * @param tile_size Size of each checkerboard tile
 */
void drawCheckerboardRectangle(ScreenBufferInfo buffer_info, int x, int y, int width, int height,
                               unsigned int color1, unsigned int color2, int tile_size) {
  if (width <= 0 || height <= 0 || tile_size <= 0) {
    return;
  }

  for (int dy = 0; dy < height; dy++) {
    int draw_y = y + dy;
    if (draw_y >= buffer_info.buffer_size[1] || draw_y < 0) {
      continue;
    }

    for (int dx = 0; dx < width; dx++) {
      int draw_x = x + dx;
      if (draw_x >= buffer_info.buffer_size[0] || draw_x < 0) {
        continue;
      }

      unsigned int pixel_color =
          (((draw_x / tile_size) + (draw_y / tile_size)) % 2 == 0) ? color1 : color2;
      *getPixelPtr(buffer_info, draw_x, draw_y) = pixel_color;
    }
  }
}

/**
 * @brief Draw a line
 * @param buffer_info Structure containing buffer information
 * @param x1 X coordinate of the line's start point
 * @param y1 Y coordinate of the line's start point
 * @param x2 X coordinate of the line's end point
 * @param y2 Y coordinate of the line's end point
 * @param color Color of the line in RGBA8888 format
 */
void drawLine(ScreenBufferInfo buffer_info, int x1, int y1, int x2, int y2, unsigned int color) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (true) {
    if (isPixelInBounds(buffer_info, x1, y1)) {
      *getPixelPtr(buffer_info, x1, y1) = color;
    }

    if (x1 == x2 && y1 == y2) {
      break;
    }

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

/**
 * @brief Draw a filled circle
 * @param buffer_info Structure containing buffer information
 * @param center_x X coordinate of the circle's center
 * @param center_y Y coordinate of the circle's center
 * @param radius Radius of the circle
 * @param color Color of the circle in RGBA8888 format
 */
void drawCircle(ScreenBufferInfo buffer_info, int center_x, int center_y, int radius,
                unsigned int color) {
  if (radius <= 0) {
    return;
  }

  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    for (int i = center_x - x; i <= center_x + x; i++) {
      if (isPixelInBounds(buffer_info, i, center_y + y)) {
        *getPixelPtr(buffer_info, i, center_y + y) = color;
      }
      if (isPixelInBounds(buffer_info, i, center_y - y)) {
        *getPixelPtr(buffer_info, i, center_y - y) = color;
      }
    }
    for (int i = center_x - y; i <= center_x + y; i++) {
      if (isPixelInBounds(buffer_info, i, center_y + x)) {
        *getPixelPtr(buffer_info, i, center_y + x) = color;
      }
      if (isPixelInBounds(buffer_info, i, center_y - x)) {
        *getPixelPtr(buffer_info, i, center_y - x) = color;
      }
    }

    y++;
    if (err <= 0) {
      err += 2 * y + 1;
      continue;
    }

    x--;
    err -= 2 * x + 1;
  }
}

/**
 * @brief Draw a gradient background
 * @param buffer_info Structure containing buffer information
 * @param start_color Starting color of the gradient
 * @param end_color Ending color of the gradient
 * @param direction Gradient direction (0: vertical, 1: horizontal)
 */
void drawGradientBackground(ScreenBufferInfo buffer_info, unsigned int start_color,
                            unsigned int end_color, int direction) {
  if (direction != 0 && direction != 1) {
    return;  // Only vertical (0) or horizontal (1) gradients are supported
  }

  int width = buffer_info.buffer_size[0];
  int height = buffer_info.buffer_size[1];
  int total_steps = (direction == 0) ? height : width;

  unsigned char start_r = (start_color >> 24) & 0xFF;
  unsigned char start_g = (start_color >> 16) & 0xFF;
  unsigned char start_b = (start_color >> 8) & 0xFF;
  unsigned char start_a = start_color & 0xFF;

  unsigned char end_r = (end_color >> 24) & 0xFF;
  unsigned char end_g = (end_color >> 16) & 0xFF;
  unsigned char end_b = (end_color >> 8) & 0xFF;
  unsigned char end_a = end_color & 0xFF;

  for (int step = 0; step < total_steps; step++) {
    float t = (float)step / (float)total_steps;

    unsigned char r = static_cast<unsigned char>(start_r + (end_r - start_r) * t);
    unsigned char g = static_cast<unsigned char>(start_g + (end_g - start_g) * t);
    unsigned char b = static_cast<unsigned char>(start_b + (end_b - start_b) * t);
    unsigned char a = static_cast<unsigned char>(start_a + (end_a - start_a) * t);

    unsigned int color = (r << 24) | (g << 16) | (b << 8) | a;

    if (direction == 0) {
      for (int x = 0; x < width; x++) {
        if (isPixelInBounds(buffer_info, x, step)) {
          *getPixelPtr(buffer_info, x, step) = color;
        }
      }
      continue;
    }

    for (int y = 0; y < height; y++) {
      if (isPixelInBounds(buffer_info, step, y)) {
        *getPixelPtr(buffer_info, step, y) = color;
      }
    }
  }
}

/**
 * @brief Draw a simple text
 * @param buffer_info Structure containing buffer information
 * @param text Text to draw
 * @param x X coordinate of the text's top-left corner
 * @param y Y coordinate of the text's top-left corner
 * @param size Size of the text
 * @param color Color of the text in RGBA8888 format
 */
void drawText(ScreenBufferInfo buffer_info, const char* text, int x, int y, int size,
              unsigned int color) {
  if (!text || size <= 0) {
    return;
  }

  const bool font[26][5][7] = {
      {{1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}},  // A
      {{1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}},  // B
      {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}},  // C
      {{1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}},  // D
      {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}},  // E
      {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}},  // F
      {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 0, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 1, 0}},  // G
      {{1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}},  // H
      {{0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}},  // I
      {{0, 0, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {0, 1, 1, 0, 0}},  // J
      {{1, 0, 0, 1, 0}, {1, 0, 1, 0, 0}, {1, 1, 0, 0, 0}, {1, 0, 1, 0, 0}, {1, 0, 0, 1, 0}},  // K
      {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}},  // L
      {{1, 0, 0, 0, 1}, {1, 1, 0, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}},  // M
      {{1, 0, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1}, {1, 0, 0, 0, 1}},  // N
      {{1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 1, 0}},  // O
      {{1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}},  // P
      {{1, 1, 1, 1, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 0}, {1, 1, 0, 1, 0}, {1, 1, 1, 0, 1}},  // Q
      {{1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 0, 0}, {1, 0, 1, 0, 0}, {1, 0, 0, 1, 0}},  // R
      {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 0, 0}, {0, 0, 0, 1, 0}, {1, 1, 1, 1, 0}},  // S
      {{1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}},  // T
      {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {0, 1, 1, 1, 0}},  // U
      {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}},  // V
      {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 1, 1}, {1, 0, 0, 0, 1}},  // W
      {{1, 0, 0, 1, 0}, {1, 0, 1, 0, 1}, {0, 1, 0, 1, 0}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 0}},  // X
      {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},  // Y
      {{1, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 0, 0}, {1, 1, 1, 1, 0}}   // Z
  };

  int scale_x = size / 7;
  int scale_y = size / 5;
  int char_spacing = size + 5;

  for (int i = 0; text[i] != '\0'; i++) {
    char c = text[i];
    int draw_x_base = x + i * char_spacing;

    if (c < 'A' || c > 'Z') {
      continue;
    }

    int char_index = c - 'A';

    for (int fy = 0; fy < 5; fy++) {
      int draw_y_base = y + fy * scale_y;

      for (int fx = 0; fx < 7; fx++) {
        if (!font[char_index][fy][fx]) {
          continue;
        }

        int char_x_base = draw_x_base + fx * scale_x;

        for (int sx = 0; sx < scale_x; sx++) {
          int draw_x = char_x_base + sx;

          for (int sy = 0; sy < scale_y; sy++) {
            int draw_y = draw_y_base + sy;

            if (isPixelInBounds(buffer_info, draw_x, draw_y)) {
              *getPixelPtr(buffer_info, draw_x, draw_y) = color;
            }
          }
        }
      }
    }
  }
}

/**
 * @brief Draw multiple colored circles
 * @param buffer_info Structure containing buffer information
 */
static void drawColoredCircles(ScreenBufferInfo buffer_info) {
  int num_circles = 20;

  for (int i = 0; i < num_circles; i++) {
    int center_x = rand() % buffer_info.buffer_size[0];
    int center_y = rand() % buffer_info.buffer_size[1];
    int radius = 10 + (rand() % 30);

    unsigned int circle_color = 0;
    switch (i % 6) {
    case 0:
      circle_color = COLOR_RED;
      break;
    case 1:
      circle_color = COLOR_GREEN;
      break;
    case 2:
      circle_color = COLOR_BLUE;
      break;
    case 3:
      circle_color = COLOR_YELLOW;
      break;
    case 4:
      circle_color = COLOR_PURPLE;
      break;
    case 5:
      circle_color = COLOR_CYAN;
      break;
    }

    drawCircle(buffer_info, center_x, center_y, radius, circle_color);
  }
}

/**
 * @brief Draw rich graphics content on the screen buffer
 * @param buffer_info Structure containing buffer information
 * @param frame_count Current frame count for animation control
 */
void drawGraphics(ScreenBufferInfo buffer_info, int frame_count) {
  // 11. Draw rich graphics content (RGBA8888 format)
  unsigned int start_color = COLOR_BLUE;
  unsigned int end_color = COLOR_PURPLE;
  if (frame_count % 100 < 50) {
    start_color = COLOR_BLUE;
    end_color = COLOR_PURPLE;
  } else {
    start_color = COLOR_PURPLE;
    end_color = COLOR_BLUE;
  }
  drawGradientBackground(buffer_info, start_color, end_color, 0);

  int rect_offset = (frame_count % 50) - 25;
  int rect_x = buffer_info.buffer_size[0] / 4 + rect_offset;
  int rect_y = buffer_info.buffer_size[1] / 4 + rect_offset;
  int rect_width = buffer_info.buffer_size[0] / 2 - (rect_offset * 2);
  int rect_height = buffer_info.buffer_size[1] / 2 - (rect_offset * 2);
  drawCheckerboardRectangle(buffer_info, rect_x, rect_y, rect_width, rect_height, COLOR_GREEN,
                            COLOR_RED, 30);

  int border_width = 10;
  drawRectangle(buffer_info, 0, 0, buffer_info.buffer_size[0], border_width, COLOR_YELLOW);
  drawRectangle(buffer_info, 0, buffer_info.buffer_size[1] - border_width,
                buffer_info.buffer_size[0], border_width, COLOR_YELLOW);
  drawRectangle(buffer_info, 0, 0, border_width, buffer_info.buffer_size[1], COLOR_YELLOW);
  drawRectangle(buffer_info, buffer_info.buffer_size[0] - border_width, 0, border_width,
                buffer_info.buffer_size[1], COLOR_YELLOW);

  int line_offset = (frame_count % 20) - 10;
  drawLine(buffer_info, line_offset, line_offset, buffer_info.buffer_size[0] - 1 - line_offset,
           buffer_info.buffer_size[1] - 1 - line_offset, COLOR_WHITE);
  drawLine(buffer_info, buffer_info.buffer_size[0] - 1 - line_offset, line_offset, line_offset,
           buffer_info.buffer_size[1] - 1 - line_offset, COLOR_WHITE);

  drawColoredCircles(buffer_info);

  int text_offset = (frame_count % 100) - 50;
  drawText(buffer_info, "HELLO QNX", 50 + text_offset, 50 + text_offset, 15, COLOR_WHITE);

  char info_text[100];
  snprintf(info_text, sizeof(info_text), "Resolution: %dx%d - Frame: %d",
           buffer_info.buffer_size[0], buffer_info.buffer_size[1], frame_count);
  drawText(buffer_info, info_text, 50, buffer_info.buffer_size[1] - 100, 8, COLOR_YELLOW);
}
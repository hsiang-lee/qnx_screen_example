#pragma once

/**
 * @brief Structure containing screen buffer information
 */
typedef struct {
  int* buffer_ptr;    /**< Pointer to the buffer memory */
  int buffer_size[2]; /**< Size of the buffer (width, height) */
  int buffer_stride;  /**< Stride (bytes per row) of the buffer */
} ScreenBufferInfo;

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
                   unsigned int color);

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
                               unsigned int color1, unsigned int color2, int tile_size);

/**
 * @brief Draw a line
 * @param buffer_info Structure containing buffer information
 * @param x1 X coordinate of the line's start point
 * @param y1 Y coordinate of the line's start point
 * @param x2 X coordinate of the line's end point
 * @param y2 Y coordinate of the line's end point
 * @param color Color of the line in RGBA8888 format
 */
void drawLine(ScreenBufferInfo buffer_info, int x1, int y1, int x2, int y2, unsigned int color);

/**
 * @brief Draw a filled circle
 * @param buffer_info Structure containing buffer information
 * @param center_x X coordinate of the circle's center
 * @param center_y Y coordinate of the circle's center
 * @param radius Radius of the circle
 * @param color Color of the circle in RGBA8888 format
 */
void drawCircle(ScreenBufferInfo buffer_info, int center_x, int center_y, int radius,
                unsigned int color);

/**
 * @brief Draw a gradient background
 * @param buffer_info Structure containing buffer information
 * @param start_color Starting color of the gradient
 * @param end_color Ending color of the gradient
 * @param direction Gradient direction (0: vertical, 1: horizontal)
 */
void drawGradientBackground(ScreenBufferInfo buffer_info, unsigned int start_color,
                            unsigned int end_color, int direction);

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
              unsigned int color);

/**
 * @brief Draw rich graphics content on the screen buffer
 * @param buffer_info Structure containing buffer information
 * @param frame_count Current frame count for animation control
 */
void drawGraphics(ScreenBufferInfo buffer_info, int frame_count);
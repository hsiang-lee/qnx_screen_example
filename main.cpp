#include <screen/screen.h>
#include <stdlib.h>

#include <iostream>

#include "draw_graphics.h"

#define MAX_DISPLAYS 5
#define DEFAULT_DISPLAY_INDEX 1

screen_context_t initialize_screen_context() {
  screen_context_t screen_context = nullptr;
  int err = screen_create_context(&screen_context, SCREEN_APPLICATION_CONTEXT);
  if (err != 0) {
    std::cerr << "Error: Failed to create screen context - " << err << std::endl;
  }
  return screen_context;
}

screen_window_t create_screen_window(screen_context_t screen_context) {
  screen_window_t screen_window = nullptr;
  int err = screen_create_window(&screen_window, screen_context);
  if (err != 0) {
    std::cerr << "Error: Failed to create window - " << err << std::endl;
  }
  return screen_window;
}

void set_window_properties(screen_window_t screen_window) {
  int usage = SCREEN_USAGE_READ | SCREEN_USAGE_WRITE;
  int err = screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_USAGE, &usage);
  if (err != 0) {
    std::cerr << "Error: Failed to set window usage - " << err << std::endl;
  }
}

void switch_display_device(screen_context_t screen_context, screen_window_t screen_window) {
  int num_displays = 0;
  int err =
      screen_get_context_property_iv(screen_context, SCREEN_PROPERTY_DISPLAY_COUNT, &num_displays);
  if (err != 0) {
    std::cerr << "Error: Failed to get display count - " << err << std::endl;
    return;
  }

  std::cout << "Display count: " << num_displays << std::endl;

  if (num_displays > DEFAULT_DISPLAY_INDEX) {
    screen_display_t displays[MAX_DISPLAYS];
    err =
        screen_get_context_property_pv(screen_context, SCREEN_PROPERTY_DISPLAYS, (void**)&displays);
    if (err == 0) {
      screen_display_t target_display = displays[DEFAULT_DISPLAY_INDEX];
      err = screen_set_window_property_pv(screen_window, SCREEN_PROPERTY_DISPLAY,
                                          (void**)&target_display);
      if (err == 0) {
        std::cout << "Window switched to display " << DEFAULT_DISPLAY_INDEX << std::endl;
      } else {
        std::cerr << "Error: Failed to switch display - " << err << std::endl;
      }
    } else {
      std::cerr << "Error: Failed to get display list - " << err << std::endl;
    }
  }
}

bool initialize_buffer(screen_window_t screen_window, screen_buffer_t& screen_buffer,
                       int buffer_size[2], int*& buffer_ptr, int& buffer_stride) {
  int err = screen_create_window_buffers(screen_window, 1);
  if (err != 0) {
    std::cerr << "Error: Failed to create window buffer - " << err << std::endl;
    return false;
  }

  err = screen_get_window_property_iv(screen_window, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
  if (err != 0) {
    std::cerr << "Error: Failed to get buffer size - " << err << std::endl;
    return false;
  }

  err = screen_get_window_property_pv(screen_window, SCREEN_PROPERTY_RENDER_BUFFERS,
                                      (void**)&screen_buffer);
  if (err != 0) {
    std::cerr << "Error: Failed to get buffer handle - " << err << std::endl;
    return false;
  }

  err = screen_get_buffer_property_pv(screen_buffer, SCREEN_PROPERTY_POINTER, (void**)&buffer_ptr);
  if (err != 0) {
    std::cerr << "Error: Failed to get buffer pointer - " << err << std::endl;
    return false;
  }

  err = screen_get_buffer_property_iv(screen_buffer, SCREEN_PROPERTY_STRIDE, &buffer_stride);
  if (err != 0) {
    std::cerr << "Error: Failed to get buffer stride - " << err << std::endl;
    return false;
  }

  return true;
}

void cleanup_resources(screen_window_t screen_window, screen_context_t screen_context) {
  if (screen_window) {
    screen_destroy_window(screen_window);
  }
  if (screen_context) {
    screen_destroy_context(screen_context);
  }
}

int main() {
  screen_context_t screen_context = initialize_screen_context();
  if (screen_context == nullptr) {
    return EXIT_FAILURE;
  }

  screen_window_t screen_window = create_screen_window(screen_context);
  if (screen_window == nullptr) {
    cleanup_resources(nullptr, screen_context);
    return EXIT_FAILURE;
  }

  set_window_properties(screen_window);
  switch_display_device(screen_context, screen_window);

  screen_buffer_t screen_buffer = nullptr;
  int buffer_size[2] = {0, 0};
  int* buffer_ptr = nullptr;
  int buffer_stride = 0;

  if (!initialize_buffer(screen_window, screen_buffer, buffer_size, buffer_ptr, buffer_stride)) {
    cleanup_resources(screen_window, screen_context);
    return EXIT_FAILURE;
  }

  for (int i = 0; i < 500; i++) {
    ScreenBufferInfo buffer_info;
    buffer_info.buffer_ptr = buffer_ptr;
    buffer_info.buffer_size[0] = buffer_size[0];
    buffer_info.buffer_size[1] = buffer_size[1];
    buffer_info.buffer_stride = buffer_stride;

    drawGraphics(buffer_info, i);

    int err = screen_post_window(screen_window, screen_buffer, 0, NULL, 0);
    if (err != 0) {
      std::cerr << "screen_post_window failed: " << err << std::endl;
      break;
    }
    usleep(40000);
  }

  cleanup_resources(screen_window, screen_context);

  return EXIT_SUCCESS;
}
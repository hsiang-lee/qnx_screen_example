# QNX screen Drawing Example Project

[Switch to Chinese README](README.md)

## Project Introduction

This is an example project demonstrating screen drawing using the QNX screen library in a QNX system. The project shows how to successfully display graphical content in a QNX system within a QNX-Android dual system environment by switching to the correct display device.

## Background Story

In a QNX-Android dual system environment, the default display shows the Android interface. There is abundant information online about using the QNX screen library to draw on the screen in QNX, but many developers struggle to get it working following tutorials. Even when compiling and running official example code directly, it often fails to display anything.

After extensive exploration and debugging, the author discovered the root cause: the system has multiple display devices. By default, applications use display 0, but in dual system environments, this default display may not show content properly. The solution is to switch the application to another available display device.

This example code demonstrates how to first retrieve all display devices in the system, then switch the application to a specified display (using display 1 by default), and finally draw rich graphical content using the screen library.

## Features

This example code implements the following features:

1. Create screen context and window using the screen library
2. Get the number of available display devices in the system
3. Switch the application to a specified display device (uses display 1 by default)
4. Create window buffer and retrieve related information
5. Draw rich graphical content including:
   - Gradient background with color transition animation
   - Central checkerboard rectangle with movement animation
   - Colored border
   - Diagonal lines with offset animation
   - Multiple colored circles at random positions
   - Pixel art style "HELLO QNX" text with movement animation
   - Resolution and frame count information display
6. Display the drawn content and enter animation loop

## Compilation Method

### Prerequisites

- QNX SDP development environment installed
- Cross-compilation toolchain configured

### Compilation Steps

1. Ensure the `CMakeLists.txt` file exists in the current directory
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
3. Run CMake to generate Makefile:
   ```bash
   cmake ..
   ```
4. Compile the project:
   ```bash
   make
   ```
5. After successful compilation, the executable `qnx_display` will be generated

## Usage Instructions

1. Run the compiled executable on QNX system:
   ```bash
   ./qnx_display
   ```

2. Observe if the graphical content is displayed on the screen

3. If no content is displayed, you may need to modify the `DEFAULT_DISPLAY_INDEX` macro definition in the code to try different display devices:
   ```cpp
   // Default display device index (0-based)
   #define DEFAULT_DISPLAY_INDEX 1  // Try changing to 0, 2, etc.
   ```

4. Recompile and run until you find a display device that works correctly

## Notes

1. In this dual system example, the default display 0 cannot show content properly, while switching to display 1 works correctly. However, this may vary in different system environments and needs to be adjusted according to actual conditions.

2. You can check the number of available display devices by observing the program output: "Display count: X", then try using index values between 0 and X-1.

3. The code contains an animation loop of 500 frames, which will exit automatically after completion without manual termination.

4. This example is for demonstration purposes only. In practical applications, proper event handling and resource release mechanisms should be added.

## Code Structure

- `main.cpp`: Main program file containing all screen library calls and animation loop logic
- `draw_graphics.h`: Header file for graphics drawing functionality, defining interfaces for various graphics drawing functions
- `draw_graphics.cpp`: Implementation file for graphics drawing functionality, containing specific implementations of various graphics drawing functions
- `CMakeLists.txt`: CMake build configuration file

## Extension Suggestions

1. Add automatic display device detection functionality to avoid manual index modification
2. Implement more complex graphics drawing features
3. Add touch event handling
4. Implement multi-window management

## Summary

This example project solves the common problem of being unable to display content using the screen library in a QNX-Android dual system environment, providing a reliable method for screen drawing in QNX. By understanding and modifying this example, developers can quickly master QNX screen drawing techniques in dual system environments.
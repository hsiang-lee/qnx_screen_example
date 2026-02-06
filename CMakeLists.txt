cmake_minimum_required(VERSION 3.16)

project(qnx_display)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find QNX screen library
# Note: In QNX development environment, the screen library is typically found in:
# /opt/qnx7xx/target/qnx7/usr/include/screen
# /opt/qnx7xx/target/qnx7/usr/lib

# Try to find the screen library include directory
find_path(SCREEN_INCLUDE_DIR screen/screen.h
    HINTS /opt/qnx*/target/qnx*/usr/include
    DOC "QNX screen library include directory"
)

# Try to find the screen library
find_library(SCREEN_LIBRARY screen
    HINTS /opt/qnx*/target/qnx*/usr/lib
    DOC "QNX screen library"
)

if(NOT SCREEN_INCLUDE_DIR)
    message(WARNING "QNX screen library include directory not found. Please set SCREEN_INCLUDE_DIR manually.")
endif()

if(NOT SCREEN_LIBRARY)
    message(WARNING "QNX screen library not found. Please set SCREEN_LIBRARY manually.")
endif()

add_executable(${PROJECT_NAME} main.cpp draw_graphics.cpp)

# Include the screen library directory
target_include_directories(${PROJECT_NAME} PRIVATE ${SCREEN_INCLUDE_DIR})

# Link against the screen library
target_link_libraries(${PROJECT_NAME} PRIVATE ${SCREEN_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION bin
    COMPONENT ${PROJECT_NAME}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:DEBUG>:-g -O0>
    $<$<CONFIG:RELEASE>:-O3 -DNDEBUG>
)
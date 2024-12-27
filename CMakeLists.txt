cmake_minimum_required(VERSION 3.13)
project(bmp_fb)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/main.cpp

    src/bitmap_fb.cpp
    src/gl_helpers.cpp
    src/imgui_gl.cpp
    src/resizable_target.cpp
    src/sdl_gl.cpp
    src/shader.cpp
    src/shader_linker.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

set(SDL_STATIC ON)
add_subdirectory(deps/SDL)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main)

add_library(glad STATIC deps/glad_lib/src/gl.c)
target_include_directories(glad PUBLIC deps/glad_lib/include/)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

set(IMGUI_SOURCES
    # Main Imgui files
    deps/imgui/imgui.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/imgui/imgui_widgets.cpp
    # SDL2+OpenGL-specific files
    deps/imgui/backends/imgui_impl_sdl2.cpp
    deps/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
# Make SDL2 available to Imgui
target_include_directories(imgui PUBLIC deps/SDL/include/)
# imgui/backends/ contains the SDL implementation
target_include_directories(imgui PUBLIC deps/imgui/ deps/imgui/backends/)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

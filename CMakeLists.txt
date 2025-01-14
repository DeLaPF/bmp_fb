cmake_minimum_required(VERSION 3.13)
project(bmp_fb)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 20)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(build_default_ex TRUE)
    if (${BUILD_EX_AUDIO})
        add_subdirectory(examples/audio)
        set(build_default_ex FALSE)
    endif()
    if (${BUILD_EX_BASIC})
        add_subdirectory(examples/basic)
        set(build_default_ex FALSE)
    endif()
    if (${BUILD_EX_KBD})
        add_subdirectory(examples/kbd)
        set(build_default_ex FALSE)
    endif()

    if (${build_default_ex})
        add_subdirectory(examples/kbd)
    endif()
endif()

add_subdirectory(deps/SDL)

add_library(glad STATIC deps/glad_lib/src/gl.c)
target_include_directories(glad PUBLIC deps/glad_lib/include/)

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

# TODO: should only the headers be included?
# How would that be passed down to src/CMakeLists.txt?
# maybe just library of include files?
add_subdirectory(deps/spdlog)

add_subdirectory(src bmp_fb)

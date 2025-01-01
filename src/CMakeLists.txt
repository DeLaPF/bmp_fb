set(SOURCES
    bitmap_fb.cpp
    imgui_gl.cpp
    sdl_gl.cpp
    shader_compiler.cpp
    shader_linker.cpp
)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)

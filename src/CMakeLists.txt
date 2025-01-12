set(SOURCES
    BFGe/BFGe.cpp
    BFGe/application.cpp

    audio.cpp
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

# Target with engine defined entrypoint
set(PROJECT_NAME_ENTRY ${PROJECT_NAME}-entrypoint)
add_library(${PROJECT_NAME_ENTRY} STATIC ${SOURCES} BFGe/entrypoint.cpp)
add_library(${PROJECT_NAME}::Entrypoint ALIAS ${PROJECT_NAME_ENTRY})
target_include_directories(${PROJECT_NAME_ENTRY} PUBLIC .)

target_link_libraries(${PROJECT_NAME_ENTRY} PUBLIC glad)
target_link_libraries(${PROJECT_NAME_ENTRY} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME_ENTRY} PUBLIC SDL2::SDL2-static)
target_link_libraries(${PROJECT_NAME_ENTRY} PRIVATE SDL2::SDL2main)

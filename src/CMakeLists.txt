set(SOURCES
    application.cpp
    bitmap_fb.cpp
    imgui_gl.cpp
    sdl_gl.cpp
    shader_compiler.cpp
    shader_linker.cpp
)
if(BFGeEntry)
    message(STATUS "Defining Sources: adding 'entrypoint.cpp'")
    list(APPEND SOURCES entrypoint.cpp)
else()
    message(STATUS "Defining Sources: not adding 'entrypoint.cpp'")
endif()
message(STATUS "Using Sources: ${SOURCES}")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)

if (BFGeEntry)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

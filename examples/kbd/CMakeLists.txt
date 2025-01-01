set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/kbd/bin)

set(kbd_sources
    main.cpp
)
add_executable(kbd ${kbd_sources})
target_include_directories(kbd PRIVATE .)

target_link_libraries(kbd PRIVATE ${PROJECT_NAME})
target_link_libraries(kbd PRIVATE SDL2::SDL2main)

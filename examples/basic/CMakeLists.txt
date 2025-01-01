set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/basic/bin)

set(basic_sources
    main.cpp
)
add_executable(basic ${basic_sources})
target_include_directories(basic PRIVATE .)

target_link_libraries(basic PRIVATE ${PROJECT_NAME})
# target_link_libraries(basic PRIVATE glad)
# target_link_libraries(basic PRIVATE imgui)
# target_link_libraries(basic PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(basic PRIVATE SDL2::SDL2main)

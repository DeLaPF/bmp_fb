set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/basic/bin)

set(basic_sources
    app.cpp
)
add_executable(basic ${basic_sources})
target_include_directories(basic PRIVATE .)

target_link_libraries(basic PRIVATE ${PROJECT_NAME})

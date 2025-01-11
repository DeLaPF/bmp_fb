set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/audio/bin)

set(audio_sources
    app.cpp
)
add_executable(audio ${audio_sources})
target_include_directories(audio PRIVATE .)

target_link_libraries(audio PRIVATE ${PROJECT_NAME}::Entrypoint)

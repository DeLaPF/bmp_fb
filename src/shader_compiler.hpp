#pragma once

#include <string>

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

struct ParsedShader {
    std::string Vertex;
    std::string Fragment;
};

ParsedShader parseShader(const std::string& filepath);
ParsedShader parseShader(const std::string& filepath, ShaderType type);
unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

#include "shader_compiler.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/gl.h"

ParsedShader parseShader(const std::string& filepath, ShaderType type)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType curType = type;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

ParsedShader parseShader(const std::string& filepath)
{
    return parseShader(filepath, ShaderType::NONE);
}

unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(len*sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout <<
            std::format(
                "ERROR [SHADER COMPILATION]({}): {}",
                type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                message
            ) <<
            std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // TODO: error handling

    return program;
}

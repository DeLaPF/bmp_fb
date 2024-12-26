#pragma once

#include <string>
#include <vector>

struct ShaderPair {
    std::string name;
    std::string vertPath;
    std::string fragPath;
    bool isCombined;
};

std::vector<ShaderPair> linkShaders(const char* shaderDir);
std::vector<unsigned int> compileShaders(const std::vector<ShaderPair>& shaderPairs);

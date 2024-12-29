#pragma once

#include <string>
#include <vector>

struct ShaderPair {
    std::string name;
    std::string vertPath;
    std::string fragPath;
    bool isCombined;
};

std::vector<ShaderPair> linkShaders(const char* shaderDir, const std::vector<std::string>* names = nullptr);
std::vector<unsigned int> compileShaders(const std::vector<ShaderPair>& shaderPairs);

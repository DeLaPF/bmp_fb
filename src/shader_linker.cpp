#include "shader_linker.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>

#include "shader.hpp"

std::vector<ShaderPair> linkShaders(const char* shaderDir, const std::vector<std::string>* names)
{
    std::vector<ShaderPair> pairs;
    std::vector<std::filesystem::path> shaderFiles;
    std::string defaultVertPath = "";
    std::string defaultFragPath = "";
    std::filesystem::path root(shaderDir);

    for (const auto &entry : std::filesystem::directory_iterator(root)) {
        if (entry.is_regular_file()) {
            auto stem = entry.path().stem().stem().string();
            auto preExt = entry.path().stem().extension().string();
            auto ext = entry.path().extension().string();
            if (preExt == ".vert" || preExt == ".frag" || ext == ".glsl") {
                if (stem == "default") {
                    if (preExt == ".vert") {
                        defaultVertPath = (root / "default.vert.glsl").string();
                    } else if (preExt == ".frag") {
                        defaultFragPath = (root / "default.frag.glsl").string();
                    }
                } else {
                    if (names == nullptr || find(names->begin(), names->end(), stem) != names->end()) {
                        shaderFiles.push_back(entry.path());
                    }
                }
            }
        }
    }
    std::sort(shaderFiles.begin(), shaderFiles.end());
    
    for (int i = 0; i < shaderFiles.size(); i++) {
        auto curFile = shaderFiles[i];
        auto stem = curFile.stem().stem().string();
        auto preExt = curFile.stem().extension().string();
        auto ext = curFile.extension().string();
        std::string vertPath = curFile.string();
        std::string fragPath = curFile.string();
        bool isCombined = false;
        std::string linkError = "";

        if (preExt == ".frag") {
            if (i < shaderFiles.size()-1 && stem == shaderFiles[i+1].stem().stem().string()) {
                vertPath = shaderFiles[i+1].string();
                i++;
            } else if (!defaultVertPath.empty()) {
                vertPath = defaultVertPath;
            } else {
                linkError = std::format(
                    "{}.frag.glsl is missing matching {}.vert.glsl (no default.vert.glsl)",
                    stem, stem
                );
            }
        } else if (preExt == ".vert") {
            if (!defaultFragPath.empty()) {
                fragPath = defaultFragPath;
            } else {
                linkError = std::format(
                    "{}.vert.glsl is missing matching {}.frag.glsl (no default.frag.glsl)",
                    stem, stem
                );
            }
        } else if (ext == ".glsl") {
            isCombined=true;
        }

        if (!linkError.empty()) {
            std::cout << linkError << std::endl;
        } else {
            pairs.push_back(ShaderPair{
                .name=stem,
                .vertPath=vertPath,
                .fragPath=fragPath,
                .isCombined=isCombined,
            });
        }
    }

    return pairs;
}

std::vector<unsigned int> compileShaders(const std::vector<ShaderPair>& shaderPairs)
{
    std::vector<unsigned int> programIds;
    programIds.reserve(shaderPairs.size());

    for (const auto& pair : shaderPairs) {
        ParsedShader shader;
        if (pair.isCombined) {
            shader = parseShader(pair.vertPath);
        } else {
            ParsedShader temp = parseShader(pair.vertPath, ShaderType::VERTEX);
            shader = parseShader(pair.fragPath, ShaderType::FRAGMENT);
            shader.Vertex = temp.Vertex;
        }
        programIds.push_back(createShader(shader.Vertex, shader.Fragment));
    }

    return programIds;
}

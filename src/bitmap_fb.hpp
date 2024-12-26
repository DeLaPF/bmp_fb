#pragma once

#include <vector>

class BitmapFramebuffer {
public:
    BitmapFramebuffer(unsigned int width, unsigned int height);
    ~BitmapFramebuffer();

    void render();
private:
    std::vector<unsigned int> m_vertices;
    std::vector<unsigned int> m_colors;
    std::vector<unsigned int> m_indices;

    unsigned int m_vao;
    unsigned int m_vboVertex;
    unsigned int m_vboColor;
    unsigned int m_vio;
};

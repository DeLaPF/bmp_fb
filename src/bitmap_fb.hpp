#pragma once

#include <vector>

struct UVertex {
    unsigned int x;
    unsigned int y;
    unsigned int s;
    unsigned int t;
};
typedef unsigned int PVertex;

class BitmapFramebuffer {
public:
    BitmapFramebuffer(unsigned int width, unsigned int height);
    ~BitmapFramebuffer();

    void render();
private:
    unsigned int width;
    unsigned int height;
    std::vector<unsigned int> m_vertices;
    std::vector<unsigned int> m_colors;
    std::vector<unsigned int> m_indices;

    unsigned int m_vao;
    unsigned int m_vboVertex;
    unsigned int m_vboColor;
    unsigned int m_ibo;
};

extern const unsigned int MASK_X;
extern const unsigned int MASK_Y;
extern const unsigned int MASK_S;
extern const unsigned int MASK_T;
extern const unsigned int MASK_NONE;
unsigned int packVertex(UVertex vertex);
UVertex unpackVertex(PVertex pvertex);

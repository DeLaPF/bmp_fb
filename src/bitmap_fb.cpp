#include "bitmap_fb.hpp"

#include <vector>

BitmapFramebuffer::BitmapFramebuffer(unsigned int width, unsigned int height)
{
    // TODO gen and set
    std::vector<unsigned int> m_vertices;
    std::vector<unsigned int> m_colors;
    std::vector<unsigned int> m_indices;

    unsigned int m_vao;
    unsigned int m_vboVertex;
    unsigned int m_vboColor;
    unsigned int m_vio;
}

BitmapFramebuffer::~BitmapFramebuffer()
{
}

void BitmapFramebuffer::render()
{
}

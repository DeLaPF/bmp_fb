#include "bitmap_fb.hpp"

#include "glad/gl.h"
#include <bitset>
#include <format>
#include <iostream>

BitmapFramebuffer::BitmapFramebuffer(unsigned int width, unsigned int height)
{
    const unsigned int vertsPerQuad = 4;
    const unsigned int indsPerQuad = 6;
    unsigned int totalPixels = width*height;

    m_vertices.reserve(totalPixels*vertsPerQuad);
    m_colors.reserve(totalPixels*vertsPerQuad);
    m_indices.reserve(totalPixels*indsPerQuad);

    this->width = width;
    this->height = height;
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // Add vertices
            this->m_vertices.push_back(packVertex({x+0, y+1, 0, 1}));
            this->m_vertices.push_back(packVertex({x+1, y+1, 1, 1}));
            this->m_vertices.push_back(packVertex({x+1, y+0, 1, 0}));
            this->m_vertices.push_back(packVertex({x+0, y+0, 0, 0}));

            // TODO: add colors

            // Add indices
            unsigned int indOff = (y*width + x)*vertsPerQuad;
            this->m_indices.push_back(0+indOff);
            this->m_indices.push_back(1+indOff);
            this->m_indices.push_back(2+indOff);
            this->m_indices.push_back(2+indOff);
            this->m_indices.push_back(3+indOff);
            this->m_indices.push_back(0+indOff);
        }
    }
    // std::cout << "vertices: ";
    // for (const auto &num : this->m_vertices) {
    //     // std::cout << std::bitset<32>(num) << " ";
    //     UVertex uv = unpackVertex(num);
    //     std::cout << std::format("(x:{} y:{} s:{} t:{})", uv.x, uv.y, uv.s, uv.t) << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "indices:";
    // for (const auto &num : this->m_indices ) {
    //     std::cout << num << " ";
    // }
    // std::cout << std::endl;

    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    glGenBuffers(1, &this->m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(unsigned int), &this->m_vertices[0], GL_STATIC_DRAW);
    // TODO: zero dynamic color buffer data
    // may actually be better to use an SSBO (Shader Storage Buffer Object),
    // since we can avoid quadruple defining color values
    // and they support up to 128mb, currently the max bitmap is 1024x1024,
    // colors will be packed into uint32 as rgba (8-8-8-8),
    // which would only be ~5mb with 16mil colors (only support GL 4.3+)

    glGenBuffers(1, &this->m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), &this->m_indices[0], GL_STATIC_DRAW);

    // packed vertices
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, 0);
    glEnableVertexAttribArray(0);
    // TODO: add color attrib pointer
}

BitmapFramebuffer::~BitmapFramebuffer()
{
}

void BitmapFramebuffer::render()
{
    // clear current target
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render quads to current target
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glDrawElements(GL_TRIANGLES, (unsigned int)this->m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

// NOTE: must stay aligned with shader code
const unsigned int MASK_X = 0x0003FF;
const unsigned int MASK_Y = 0x0FFC00;
const unsigned int MASK_S = 0x100000;
const unsigned int MASK_T = 0x200000;
const unsigned int MASK_NONE = 0xFFC00000;
unsigned int packVertex(UVertex uVertex)
{
    unsigned int vertex = 0;
    vertex |= (uVertex.x <<  0) & MASK_X;
    vertex |= (uVertex.y << 10) & MASK_Y;
    vertex |= (uVertex.s << 20) & MASK_S;
    vertex |= (uVertex.t << 21) & MASK_T;

    return vertex;
}

UVertex unpackVertex(PVertex pVertex)
{
    unsigned int x = (pVertex&MASK_X) >>  0;
    unsigned int y = (pVertex&MASK_Y) >> 10;
    unsigned int s = (pVertex&MASK_S) >> 20;
    unsigned int t = (pVertex&MASK_T) >> 21;

    return { x, y, s, t };
}

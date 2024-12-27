#pragma once

#include <memory>
#include <vector>

struct UVertex {
    unsigned int x;
    unsigned int y;
    unsigned int s;
    unsigned int t;
};
typedef unsigned int PVertex;

typedef std::vector<unsigned int> Bitmap;

class BitmapFramebuffer {
public:
    BitmapFramebuffer(unsigned int width, unsigned int height);
    ~BitmapFramebuffer();

    std::shared_ptr<Bitmap> getBitmap();

    void updateBitmap();

    void render();
private:
    unsigned int width;
    unsigned int height;
    std::vector<unsigned int> m_vertices;
    std::vector<unsigned int> m_indices;
    std::shared_ptr<Bitmap> m_bitmap;

    unsigned int m_vao;
    unsigned int m_vboVertex;
    unsigned int m_ssboColor;
    unsigned int m_ibo;
};

extern const unsigned int MASK_X;
extern const unsigned int MASK_Y;
extern const unsigned int MASK_S;
extern const unsigned int MASK_T;
extern const unsigned int MASK_NONE;
unsigned int packVertex(UVertex vertex);
UVertex unpackVertex(PVertex pvertex);

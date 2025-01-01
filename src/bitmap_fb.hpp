#pragma once

#include <array>
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
    BitmapFramebuffer(
        unsigned int bmpW, unsigned int bmpH,
        int renderW, int renderH, unsigned int colorPackingFormat = RGBA,
        int minRW = 16, int minRH = 16
    );
    ~BitmapFramebuffer();

    std::shared_ptr<Bitmap> getBitmap();
    unsigned int getTextureId();

    void render(bool toFb = true);
    void resizeRenderDim(int w, int h);
    void updateBitmap();
    void updateModelViewMatrix();

    void bind();
    void unbind();
    void bindBase();
    void unbindBase();
    void bindFb();
    void unbindFb();

    // ColorPackingFormat Flags
    static const unsigned int RGBA = 0x1;
    static const unsigned int SINGLE_BIT = 0x2;

private:
    unsigned int m_bmpW;
    unsigned int m_bmpH;
    std::vector<unsigned int> m_vertices;
    std::vector<unsigned int> m_indices;
    std::shared_ptr<Bitmap> m_bitmap;
    std::array<float, 9> m_mvm;
    unsigned int m_colorPackingFormat;

    unsigned int m_vao;
    unsigned int m_vboVertex;
    unsigned int m_ibo;
    unsigned int m_ssboColor;

    int m_renderW;
    int m_renderH;
    int m_minRW;
    int m_minRH;
    unsigned int m_fbo;
    unsigned int m_rbo;
    unsigned int m_textureId;

    unsigned int m_shaderPg;
    int m_uBitmapDim;
    int m_uMVM;
    int m_uColorFormatFlag;
};

extern const unsigned int MASK_X;
extern const unsigned int MASK_Y;
extern const unsigned int MASK_S;
extern const unsigned int MASK_T;
extern const unsigned int MASK_NONE;
unsigned int packVertex(UVertex vertex);
UVertex unpackVertex(PVertex pvertex);

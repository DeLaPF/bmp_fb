#include "bitmap_fb.hpp"

#include <algorithm>
#include <format>
#include <iostream>

#include "glad/gl.h"

#include "shader_linker.hpp"

// Note: expects required gl objects to be bound
static void createTextureFb(int w, int h, int texId, unsigned int rbo)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR [FRAMEBUFFER]: Framebuffer is not complete!" << std::endl;
        exit(1);
    }
}

BitmapFramebuffer::BitmapFramebuffer(
    unsigned int bmpW, unsigned int bmpH,
    int renderW, int renderH, unsigned int colorPackingFormat,
    int minRW, int minRH
)
{
    const unsigned int vertsPerQuad = 4;
    const unsigned int indsPerQuad = 6;
    unsigned int totalPixels = bmpW*bmpH;
    unsigned int numPackedUint32;

    this->m_colorPackingFormat = colorPackingFormat;
    switch(colorPackingFormat) {
    case RGBA:
        numPackedUint32 = totalPixels;
        break;
    case SINGLE_BIT:
        numPackedUint32 = (totalPixels / 32) + 1;
        break;
    default:
        std::cout << std::format(
            "Error[BITMAP_FB]: Invalid color packing format '{}'",
            colorPackingFormat
        ) << std::endl;
        exit(1);
    }

    this->m_vertices.reserve(totalPixels*vertsPerQuad);
    this->m_indices.reserve(totalPixels*indsPerQuad);
    this->m_bitmap = std::make_shared<Bitmap>();
    this->m_bitmap->reserve(numPackedUint32);
    for (unsigned int i = 0; i < numPackedUint32; i++) { this->m_bitmap->push_back(0); }

    this->m_bmpW = bmpW;
    this->m_bmpH = bmpH;
    for (unsigned int y = 0; y < bmpH; y++) {
        for (unsigned int x = 0; x < bmpW; x++) {
            // Add vertices
            this->m_vertices.push_back(packVertex({x+0, y+1, 0, 1}));
            this->m_vertices.push_back(packVertex({x+1, y+1, 1, 1}));
            this->m_vertices.push_back(packVertex({x+1, y+0, 1, 0}));
            this->m_vertices.push_back(packVertex({x+0, y+0, 0, 0}));

            // Add indices
            unsigned int indOff = (y*bmpW + x)*vertsPerQuad;
            this->m_indices.push_back(0+indOff);
            this->m_indices.push_back(1+indOff);
            this->m_indices.push_back(2+indOff);
            this->m_indices.push_back(2+indOff);
            this->m_indices.push_back(3+indOff);
            this->m_indices.push_back(0+indOff);
        }
    }

    // Vertex data setup
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    glGenBuffers(1, &this->m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size()*sizeof(unsigned int), &this->m_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &this->m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size()*sizeof(unsigned int), &this->m_indices[0], GL_STATIC_DRAW);

    // uint32 packed vertices
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex data unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Dynamic Color ssbo setup
    glCreateBuffers(1, &this->m_ssboColor);
    glNamedBufferStorage(this->m_ssboColor, this->m_bitmap->size()*sizeof(unsigned int), (const void *)0, GL_DYNAMIC_STORAGE_BIT);

    // Framebuffer setup
    this->m_renderW = renderW;
    this->m_renderH = renderH;
    this->m_minRW = minRW;
    this->m_minRH = minRH;
    glGenFramebuffers(1, &this->m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
    glGenTextures(1, &this->m_textureId);
    glBindTexture(GL_TEXTURE_2D, this->m_textureId);
    glGenRenderbuffers(1, &this->m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_rbo);
    createTextureFb(this->m_renderW, this->m_renderH, this->m_textureId, this->m_rbo);

    // Framebuffer unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Shader setup
    const char* relPath = "res/shaders";
    const std::vector<std::string> names = { "bmp_fb" };
    std::vector<ShaderPair> pairs = linkShaders(relPath, &names);
    std::vector<unsigned int> shaderIds = compileShaders(pairs);
    if (shaderIds.size() < 1) {
        std::cout << std::format(
            "ERROR [Shaders]: no valid shaders found at: {}",
            relPath
        ) << std::endl;
        exit(1);
    }
    this->m_shaderPg = shaderIds[0];
    glUseProgram(this->m_shaderPg);
    this->m_uMVM = glGetUniformLocation(this->m_shaderPg, "u_ModelViewMat");
    this->m_uBitmapDim = glGetUniformLocation(this->m_shaderPg, "u_BitmapDim");
    this->m_uColorFormatFlag = glGetUniformLocation(this->m_shaderPg, "u_ColorFormatFlag");

    // Shader unbind
    glUseProgram(0);
}

BitmapFramebuffer::~BitmapFramebuffer()
{
    glDeleteProgram(this->m_shaderPg);
}

std::shared_ptr<Bitmap> BitmapFramebuffer::getBitmap()
{
    return this->m_bitmap;
}

unsigned int BitmapFramebuffer::getTextureId()
{
    return this->m_textureId;
}

void BitmapFramebuffer::render(bool toFb)
{
    this->bindBase();
    if (toFb) {
        this->bindFb();
    }

    glUniform1ui(this->m_uColorFormatFlag, this->m_colorPackingFormat);
    glUniform2ui(this->m_uBitmapDim, this->m_bmpW, this->m_bmpH);
    glUniformMatrix3fv(this->m_uMVM, 1, GL_TRUE, &this->m_mvm[0]);

    glViewport(0, 0, this->m_renderW, this->m_renderH);
    // TODO: enable setting clear color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, (unsigned int)this->m_indices.size(), GL_UNSIGNED_INT, nullptr);

    this->unbind();
}

void BitmapFramebuffer::resizeRenderDim(int w, int h)
{
    this->bind();

    this->m_renderW = std::max(w, this->m_minRW);
    this->m_renderH = std::max(h, this->m_minRH);
    this->updateModelViewMatrix();
    createTextureFb(this->m_renderW, this->m_renderH, this->m_textureId, this->m_rbo);

    this->unbind();
}

void BitmapFramebuffer::updateBitmap()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->m_ssboColor);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->m_bitmap->size()*sizeof(unsigned int), &this->m_bitmap->at(0));
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void BitmapFramebuffer::updateModelViewMatrix()
{
    int w = this->m_renderW;
    int h = this->m_renderH;
    unsigned int mW = this->m_bmpW;
    unsigned int mH = this->m_bmpH;

    float aspRW = w >= h ? ((float)w)/h : 1.0f;
    float aspRH = w >= h ? 1.0f : ((float)h)/w;
    float transW = -1.0f + (w >= h ? (w-h)/((float)w) : 0.0f);
    float transH =  1.0f + (w >= h ? 0.0f : (w-h)/((float)h));

    this->m_mvm = {
        2.0f/(mW*aspRW), 0.0f, transW,
        0.0f, -2.0f/(mH*aspRH), transH,
        0.0f, 0.0f, 0.0f
    };
}

void BitmapFramebuffer::bind()
{
    this->bindBase();
    this->bindFb();
}

void BitmapFramebuffer::unbind()
{
    this->unbindBase();
    this->unbindFb();
}

void BitmapFramebuffer::bindBase()
{
    // Bitmap (vertices/indices and color)
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->m_ssboColor);

    // Shader
    glUseProgram(this->m_shaderPg);
}

void BitmapFramebuffer::unbindBase()
{
    // Bitmap (vertices/indices and color)
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

    // Shader
    glUseProgram(0);
}

void BitmapFramebuffer::bindFb()
{
    // Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
    glBindTexture(GL_TEXTURE_2D, this->m_textureId);
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_rbo);
}

void BitmapFramebuffer::unbindFb()
{
    // Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

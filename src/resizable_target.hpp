#pragma once

class ResizeableTarget {
public:
    ResizeableTarget(int width, int height);

    void bind();
    void unbind();
    unsigned int getTextureId();
    void resize(int width, int height);
private:
    unsigned int fbo, rbo, texture_id;
    int height, width;
};

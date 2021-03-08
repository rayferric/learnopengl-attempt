#ifndef EYESPARK_TEXTURE2D_H
#define EYESPARK_TEXTURE2D_H

#include "../../resource/texture/Texture.h"

class Texture2D : public Texture {
protected:
    Texture2D() {}
public:
    void bind() override {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
};

#endif // EYESPARK_TEXTURE2D_H

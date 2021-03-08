#ifndef EYESPARK_CUBEMAPTEXTURE_H
#define EYESPARK_CUBEMAPTEXTURE_H

#include "../../resource/texture/Texture.h"

class CubeMapTexture : public Texture {
protected:
    int resolution;

    CubeMapTexture() {}
public:
    void bind() override {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    }

    int getResolution() {
        return resolution;
    }
};

#endif // EYESPARK_CUBEMAPTEXTURE_H

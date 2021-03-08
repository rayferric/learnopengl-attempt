#ifndef EYESPARK_TEXTURE_H
#define EYESPARK_TEXTURE_H

enum TextureFilter { NEAREST, BILINEAR, TRILINEAR };

class Texture {
protected:
    unsigned int texture;

    Texture() {
        glGenTextures(1, &texture);
    }
public:
    virtual void bind() = 0;

    virtual ~Texture() {
        glDeleteTextures(1, &texture);
    }
};

#endif // EYESPARK_TEXTURE_H

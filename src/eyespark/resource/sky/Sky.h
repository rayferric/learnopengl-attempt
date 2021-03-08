#ifndef EYESPARK_SKY_H
#define EYESPARK_SKY_H

class Sky {
protected:
    Shader *shader;
    unsigned int irradiance;

    Sky(Shader *shader, int irradianceSize) {
        this->irradianceSize = irradianceSize;
    }
public:
    int irradianceSize;

    virtual void update() = 0;
    void bindIrradiance() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance);
    }
};

#endif // EYESPARK_SKY_H

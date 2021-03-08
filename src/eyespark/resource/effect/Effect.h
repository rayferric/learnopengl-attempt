#ifndef EYESPARK_EFFECT_H
#define EYESPARK_EFFECT_H

#include "../../resource/Shader.h"
#include "../../util/Matrix4.h"

class Effect {
protected:
    Shader *shader;
public:
    Effect(Shader *shader) {
        this->shader = shader;
        shader->bind();

        Shader::setUniform(shader->getUniformLocation("gPosition"), 0);
        Shader::setUniform(shader->getUniformLocation("gNormal"), 1);
        Shader::setUniform(shader->getUniformLocation("gAlbedo"), 2);
        Shader::setUniform(shader->getUniformLocation("gMRA"), 3);
        Shader::setUniform(shader->getUniformLocation("gEmission"), 4);
        Shader::setUniform(shader->getUniformLocation("gColor"), 5);
    }

    virtual void bind(Matrix4 projectionMatrix, Matrix4 viewMatrix, Vector3 cameraPosition) {
        shader->bind();
    }

    ~Effect() {
        delete shader;
    }
};

#endif // EYESPARK_EFFECT_H

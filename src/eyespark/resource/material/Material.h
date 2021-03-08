#ifndef EYESPARK_MATERIAL_H
#define EYESPARK_MATERIAL_H

#include "../../resource/Shader.h"
#include "../../util/Matrix4.h"

class Material {
private:
    int uModelMatrixLocation;
    int uViewMatrixLocation;
    int uProjectionMatrixLocation;
protected:
    Shader *shader;

    Material(Shader *shader) {
        this->shader = shader;
        shader->bind();

        uModelMatrixLocation = shader->getUniformLocation("uModelMatrix");
        uViewMatrixLocation = shader->getUniformLocation("uViewMatrix");
        uProjectionMatrixLocation = shader->getUniformLocation("uProjectionMatrix");
    }
public:
    bool cullFace = true;

    virtual void bind(Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projectionMatrix) {
        shader->bind();

        Shader::setUniform(uModelMatrixLocation, modelMatrix);
        Shader::setUniform(uViewMatrixLocation, viewMatrix);
        Shader::setUniform(uProjectionMatrixLocation, projectionMatrix);
    }

    ~Material() {
        delete shader;
    }
};

#endif // EYESPARK_MATERIAL_H

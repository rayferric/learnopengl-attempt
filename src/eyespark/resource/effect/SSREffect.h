#ifndef EYESPARK_SSREFFECT_H
#define EYESPARK_SSREFFECT_H

#include "../../resource/effect/Effect.h"
#include "../../resource/Shader.h"
#include "../../util/Matrix4.h"

class SSREffect : public Effect {
private:
    int uViewMatrixLocation;
    int uProjectionMatrixLocation;
    int uCameraPositionLocation;
public:
    SSREffect() : Effect(new Shader("shaders/ssr.glsl")) {
        uViewMatrixLocation = shader->getUniformLocation("uViewMatrix");
        uProjectionMatrixLocation = shader->getUniformLocation("uProjectionMatrix");
        uCameraPositionLocation = shader->getUniformLocation("uCameraPosition");
    }

    void bind(Matrix4 projectionMatrix, Matrix4 viewMatrix, Vector3 cameraPosition) override {
        Effect::bind(projectionMatrix, viewMatrix, cameraPosition);

        Shader::setUniform(uViewMatrixLocation, viewMatrix);
        Shader::setUniform(uProjectionMatrixLocation, projectionMatrix);
        Shader::setUniform(uCameraPositionLocation, cameraPosition);
    }
};

#endif // EYESPARK_SSREFFECT_H

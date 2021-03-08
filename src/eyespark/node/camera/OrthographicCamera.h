#ifndef EYESPARK_ORTHOGRAPHICCAMERA_H
#define EYESPARK_ORTHOGRAPHICCAMERA_H

#include "../../node/camera/Camera.h"
#include "../../util/Matrix4.h"

class OrthographicCamera : public Camera {
public:
    float size;

    OrthographicCamera(float size, float nearClip, float farClip) : Camera(nearClip, farClip) {
        this->size = size;

        updateProjection();
    }
    void updateProjection() override {
            projectionMatrix = Matrix4::ortho(size * aspect, size, nearClip, farClip);
    }
};

#endif // EYESPARK_ORTHOGRAPHICCAMERA_H

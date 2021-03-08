#ifndef EYESPARK_PERSPECTIVECAMERA_H
#define EYESPARK_PERSPECTIVECAMERA_H

#include "../../node/camera/Camera.h"
#include "../../util/Matrix4.h"

class PerspectiveCamera : public Camera {
public:
    float fov;

    PerspectiveCamera(float fov, float nearClip, float farClip) : Camera(nearClip, farClip) {
        this->fov = fov;

        updateProjection();
    }
    void updateProjection() override {
            projectionMatrix = Matrix4::perspective(fov, aspect, nearClip, farClip);
    }
};

#endif // EYESPARK_PERSPECTIVECAMERA_H

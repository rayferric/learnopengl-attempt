#ifndef EYESPARK_CAMERA_H
#define EYESPARK_CAMERA_H

#include "../../node/Node.h"
#include "../../util/Matrix4.h"

class Camera : public Node {
protected:
    Matrix4 projectionMatrix;

    Camera(float nearClip, float farClip) {
        this->nearClip = nearClip;
        this->farClip = farClip;
    }
public:
    float aspect = 1.0;
    float nearClip;
    float farClip;

    Matrix4 getProjection() {
        return projectionMatrix;
    }
    virtual void updateProjection() = 0;
};

#endif // EYESPARK_CAMERA_H

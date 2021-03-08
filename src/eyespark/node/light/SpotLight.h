#ifndef EYESPARK_SPOTLIGHT_H
#define EYESPARK_SPOTLIGHT_H

#include "../../node/light/PointLight.h"
#include "../../util/Vector3.h"

class SpotLight : public PointLight {
public:
    float angle;
    SpotLight(Vector3 color, Vector3 ambient, float range, float angle) : PointLight(color, ambient, range) {
        this->angle = angle;
    }
};

#endif // EYESPARK_SPOTLIGHT_H

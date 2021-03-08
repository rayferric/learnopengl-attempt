#ifndef EYESPARK_POINTLIGHT_H
#define EYESPARK_POINTLIGHT_H

#include "../../node/light/Light.h"
#include "../../util/Vector3.h"

class PointLight : public Light {
public:
    float range;
    PointLight(Vector3 color, Vector3 ambient, float range) : Light(color, ambient) {
        this->range = range;
    }
};

#endif // EYESPARK_POINTLIGHT_H

#ifndef EYESPARK_DIRECTIONALLIGHT_H
#define EYESPARK_DIRECTIONALLIGHT_H

#include "../../node/light/Light.h"
#include "../../util/Vector3.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(Vector3 color, Vector3 ambient) : Light(color, ambient) {}
};

#endif // EYESPARK_DIRECTIONALLIGHT_H

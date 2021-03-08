#ifndef EYESPARK_LIGHT_H
#define EYESPARK_LIGHT_H

#include "../../node/Node.h"
#include "../../util/Vector3.h"

class Light : public Node {
protected:
    Light(Vector3 color, Vector3 ambient) {
        this->color = color;
        this->ambient = ambient;
    }
public:
    Vector3 color;
    Vector3 ambient;
};

#endif // EYESPARK_LIGHT_H

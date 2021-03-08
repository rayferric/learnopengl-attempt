#ifndef EYESPARK_MODEL_H
#define EYESPARK_MODEL_H

#include "../node/Node.h"
#include "../resource/material/Material.h"
#include "../resource/mesh/Mesh.h"

class Model : public Node {
public:
    Mesh *mesh;
    Material *material;

    Model(Mesh *mesh, Material *material) {
        this->mesh = mesh;
        this->material = material;
    }
};

#endif // EYESPARK_MODEL_H

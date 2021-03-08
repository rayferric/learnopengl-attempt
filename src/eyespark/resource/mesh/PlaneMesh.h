#ifndef EYESPARK_PLANEMESH_H
#define EYESPARK_PLANEMESH_H

#include "../../resource/mesh/Mesh.h"
#include "../../util/Vector2.h"

class PlaneMesh : public Mesh {
public:
    PlaneMesh(Vector2 size) {
        size *= 0.5;

        unsigned int vertexCount = 4;
        float *vertexData = new float[vertexCount*8] {
                -size.x, -size.y, 0, 0, 0, 0, 0, 1,
                 size.x, -size.y, 0, 1, 0, 0, 0, 1,
                 size.x,  size.y, 0, 1, 1, 0, 0, 1,
                -size.x,  size.y, 0, 0, 1, 0, 0, 1
        };
        indexCount = 6;
        unsigned int *indices = new unsigned int[indexCount] {
                0, 1, 2,
                2, 3, 0
        };



        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(float)*8, vertexData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void *)(sizeof(float)*3));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void *)(sizeof(float)*5));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);

        delete[] vertexData;
        delete[] indices;
    }
};

#endif // EYESPARK_PLANEMESH_H

#ifndef EYESPARK_CUBEMESH_H
#define EYESPARK_CUBEMESH_H

#include "../../resource/mesh/Mesh.h"
#include "../../util/Math.h"
#include "../../util/Vector3.h"

class CubeMesh : public Mesh {
public:
    CubeMesh(Vector3 size, bool smooth) {
        size *= 0.5;

        unsigned int vertexCount;
        float *vertexData;
        unsigned int *indices;
        if(smooth) {
            float s = Math::sqrt(1 / 3.0);
            vertexCount = 8;
            vertexData = new float[vertexCount*8] {
                    -size.x,-size.y, size.z, 0, 0,-s,-s, s,
                     size.x,-size.y, size.z, 1, 0, s,-s, s,
                     size.x, size.y, size.z, 1, 1, s, s, s,
                    -size.x, size.y, size.z, 0, 1,-s, s, s,
                    -size.x,-size.y,-size.z, 0, 0,-s,-s,-s,
                     size.x,-size.y,-size.z, 1, 0, s,-s,-s,
                     size.x, size.y,-size.z, 1, 1, s, s,-s,
                    -size.x, size.y,-size.z, 0, 1,-s, s,-s
            };
            indexCount = 36;
            indices = new unsigned int[indexCount] {
                    0, 1, 2, 2, 3, 0,
                    4, 7, 6, 6, 5, 4,
                    1, 5, 6, 6, 2, 1,
                    0, 3, 7, 7, 4, 0,
                    3, 2, 6, 6, 7, 3,
                    0, 4, 5, 5, 1, 0
            };
        } else {
            vertexCount = 24;
            vertexData = new float[vertexCount*8] {
                    -size.x,-size.y, size.z, 0, 0, 0, 0, 1,
                     size.x,-size.y, size.z, 1, 0, 0, 0, 1,
                     size.x, size.y, size.z, 1, 1, 0, 0, 1,
                    -size.x, size.y, size.z, 0, 1, 0, 0, 1,
                    -size.x,-size.y,-size.z, 0, 0, 0, 0,-1,
                     size.x,-size.y,-size.z, 1, 0, 0, 0,-1,
                     size.x, size.y,-size.z, 1, 1, 0, 0,-1,
                    -size.x, size.y,-size.z, 0, 1, 0, 0,-1,

                     size.x,-size.y, size.z, 0, 0, 1, 0, 0,
                     size.x,-size.y,-size.z, 1, 0, 1, 0, 0,
                     size.x, size.y,-size.z, 1, 1, 1, 0, 0,
                     size.x, size.y, size.z, 0, 1, 1, 0, 0,
                    -size.x,-size.y, size.z, 0, 0,-1, 0, 0,
                    -size.x,-size.y,-size.z, 1, 0,-1, 0, 0,
                    -size.x, size.y,-size.z, 1, 1,-1, 0, 0,
                    -size.x, size.y, size.z, 0, 1,-1, 0, 0,

                    -size.x, size.y, size.z, 0, 0, 0, 1, 0,
                     size.x, size.y, size.z, 1, 0, 0, 1, 0,
                     size.x, size.y,-size.z, 1, 1, 0, 1, 0,
                    -size.x, size.y,-size.z, 0, 1, 0, 1, 0,
                    -size.x,-size.y, size.z, 0, 0, 0,-1, 0,
                     size.x,-size.y, size.z, 1, 0, 0,-1, 0,
                     size.x,-size.y,-size.z, 1, 1, 0,-1, 0,
                    -size.x,-size.y,-size.z, 0, 1, 0,-1, 0
            };
            indexCount = 36;
            indices = new unsigned int[indexCount] {
                     0,  1,  2,  2,  3,  0,
                     4,  7,  6,  6,  5,  4,
                     8,  9, 10, 10, 11,  8,
                    12, 15, 14, 14, 13, 12,
                    16, 17, 18, 18, 19, 16,
                    20, 23, 22, 22, 21, 20
            };
        }

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

#endif // EYESPARK_CUBEMESH_H

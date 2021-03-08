#ifndef EYESPARK_ARRAYMESH_H
#define EYESPARK_ARRAYMESH_H

#include "../../resource/mesh/Mesh.h"
#include "../../util/Error.h"
#include "../../util/Logger.h"
#include "../../util/String.h"
#include "../../util/WinAPI.h"

class ArrayMesh : public Mesh {
private:
public:
    ArrayMesh(String path) {
        FILE* file = fopen(path.data(), "rb");
        if(file == nullptr) {
            String args[] = { WinAPI::fullPath(path) };
            Error::error(Error::FILE_OPENING_ERROR, 1, args);
        }

        unsigned int info[2];
        fread(info, sizeof(unsigned int), 2, file);
        indexCount = info[1];

        float *vertexData = new float[info[0] * 11];
        unsigned int *indices = new unsigned int[indexCount];
        fread(vertexData, sizeof(float), info[0] * 11, file);
        fread(indices, sizeof(unsigned int), indexCount, file);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, info[0]*11*sizeof(float), vertexData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const void *)(sizeof(float)*3));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const void *)(sizeof(float)*5));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const void *)(sizeof(float)*8));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);

        delete[] vertexData;
        delete[] indices;
    }
};

#endif // EYESPARK_ARRAYMESH_H

#ifndef EYESPARK_MESH_H
#define EYESPARK_MESH_H

class Mesh {
protected:
    unsigned int vao;
    unsigned int vbos[2];
    unsigned int indexCount;

    Mesh() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(2, vbos);
        glBindVertexArray(vao);
    }
public:
    void bind() {
        glBindVertexArray(vao);
    }
    unsigned int getIndexCount() {
        return indexCount;
    }
    ~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(2, vbos);
    }
};

#endif // EYESPARK_MESH_H

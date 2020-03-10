#ifndef MECH_H
#define MECH_H

#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void createMesh(GLfloat *vertices, unsigned int *indeces, unsigned int numOfVertices, unsigned int numOfIndices);
    void renderMesh();
    void clearMesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;//количесво "элементов" или индексов (ссылок на вершины)
};

#endif // MECH_H

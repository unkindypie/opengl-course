#include "mesh.h"

Mesh::Mesh() {
  VAO = 0;
  VBO = 0;
  IBO = 0;
  indexCount = 0;
}

void Mesh::createMesh(GLfloat *vertices, unsigned int *indeces,
                      unsigned int numOfVertices, unsigned int numOfIndices) {
  indexCount = numOfIndices;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces[0]) * numOfIndices,
               indeces, GL_STATIC_DRAW);

  //создает буфер для этого массива вертексов в памяти видюхи. Биндится он к
  //самому последнему созданному VAO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices,
               GL_STATIC_DRAW);
  //атрибуты, которые говорят opengl что такое этот массив верктесов позиций
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
  glEnableVertexAttribArray(0); //включаю его
  // вертексы текстур
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5,
                        (void *)(sizeof(vertices[0]) * 3));
  glEnableVertexAttribArray(1); // включаю второй

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0); // 0 - дальнейшие операции с вертксными буферами уже не
                        // будут биндится к этому буферу
}

void Mesh::renderMesh() {
  if (indexCount == 0)
    return;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::clearMesh() {
  if (IBO != 0) {
    glDeleteBuffers(1, &IBO);
    IBO = 0;
  }

  if (VBO != 0) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }

  indexCount = 0;
}

Mesh::~Mesh() { clearMesh(); }

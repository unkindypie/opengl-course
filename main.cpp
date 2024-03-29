//#define GLEW_STATIC
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = M_PI / 180;

Window *mainWindow = nullptr;
Camera *camera = nullptr;

Texture brickTexture;
Texture dirtTexture;

std::vector<Mesh *> meshlist;
std::vector<class ::Shader *> shaderlist;

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;

// Vertex shader
static const char *vShader = "./shaders/vert.glsl";
// Fragment shader
static const char *fShader = "./shaders/frag.glsl";

void createObjects() {
  //индесксы для буфера индексов нужны для того, чтобы не
  //копипастить вершины по нескольку раз, т.к. они часто повторяются в 3д
  //фигурах по сути они ссылаются на вершины
  unsigned int indeces[] = {
      0, 3, 1, // 1 сторона
      1, 3, 2, // 2 сторона
      2, 3, 0, // 3 сторона
      0, 1, 2  //дно (прямо как я)
  };

  GLfloat vertices[] = {
      // X      y     z    u   v
      -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom left
      0,     -1,    1,    0.5f, 0.0f, // Back
      2.0f,  -1.0f, 0.0f, 1.0f, 0.0f, //  Bottom right
      0.0f,  1.0f,  0.0f, 0.5f, 1.0f, // Top
  };

  meshlist.push_back(new Mesh());
  meshlist[meshlist.size() - 1]->createMesh(vertices, indeces, 20, 12);

  meshlist.push_back(new Mesh());
  meshlist[meshlist.size() - 1]->createMesh(vertices, indeces, 20, 12);
}

void createShaders() {
  class ::Shader *shader1 = new class ::Shader();
  shader1->createFromFiles(vShader, fShader);
  shaderlist.push_back(shader1);
}

void printMat(glm::mat4 matrix) {
  std::string str = glm::to_string(matrix);
  size_t pos;
  int skips = 0;
  do {
    pos = str.find_first_of(',');

    if (pos > str.size())
      break;

    if (skips == 3) {
      str.replace(pos, 1, "\n");
      skips = 0;
    } else {
      str.replace(pos, 1, " ");
      skips++;
    }
  } while (pos);

  std::cout << str << std::endl;
}

int main(int argc, char *argv[]) {
  mainWindow = new Window(800, 600);
  mainWindow->initialize();
  camera = new Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f,
                      0.f, 3.f, 0.3f);
  brickTexture = Texture("textures/brick.png");
  brickTexture.loadTexture();
  dirtTexture = Texture("textures/dirt.png");
  dirtTexture.loadTexture();

  createObjects();
  createShaders();

  GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;
  glm::mat4 projection =
      glm::perspective(45.f,
                       (GLfloat)mainWindow->getBufferWidth() /
                           (GLfloat)mainWindow->getBufferHeight(),
                       0.1f, 100.f);

  // Loop until window closed
  while (!mainWindow->getShouldClose()) {
    GLfloat now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    // Get + Handle user input events
    glfwPollEvents();

    camera->keyControl(mainWindow->getKeys(), deltaTime);
    camera->mouseControl(mainWindow->getXChange(), mainWindow->getYChange());

    // Clear window
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //говорю, к какому шейдеру сейчас отправлять вызовы всех шейдерных функций
    shaderlist[0]->useShader();
    // позиции юниформов в памяти видюхи
    uniformModel = shaderlist[0]->GetModelLocation();
    uniformProjection = shaderlist[0]->GetProjectionLocation();
    uniformView = shaderlist[0]->GetViewLocation();

    //создаю матрицу преобразований
    glm::mat4 model(1.0f); //конструктор еденичной матрицы
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0, 1, 0));
    // model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    //задаю юниформ матрицы преобразований, в который отправляю указатель на
    //матрицу преобразований
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //юниформ матрицы проекции
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                       glm::value_ptr(projection));
    // юниформ матрицы вида
    glUniformMatrix4fv(uniformView, 1, GL_FALSE,
                       glm::value_ptr(camera->calculateViewMatrix()));
    brickTexture.useTexture();
    meshlist[0]->renderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(shaderlist[0]->GetModelLocation(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshlist[1]->renderMesh();

    glUseProgram(0); //отключаю шейдер

    mainWindow->swapBuffers();
  }
  delete mainWindow;
  delete camera;
  return 0;
}

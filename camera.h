#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <stdio.h>

class Camera
{
public:
  Camera(
      glm::vec3 startPosition,
      glm::vec3 startUp,
      GLfloat startYaw,
      GLfloat startPitch,
      GLfloat startMoveSpeed,
      GLfloat startTurnSpeed);
  ~Camera();

  void keyControl(bool* keys, GLfloat deltaTime);
  void mouseControl(GLfloat xChange, GLfloat yChange);

  glm::mat4 calculateViewMatrix();

  void update();

private:
  glm::vec3 position;

  glm::vec3 front; // forward "Z"
  glm::vec3 up;
  glm::vec3 right;

  glm::vec3 worldUp;

  // повороты по
  GLfloat yaw;   // лево право (вокруг оси y)
  GLfloat pitch; //верх низ (вокруг оси x)

  GLfloat moveSpeed;
  GLfloat turnSpeed;

};
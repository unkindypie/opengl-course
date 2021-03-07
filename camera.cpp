#include "camera.h"

Camera::Camera(
    glm::vec3 startPosition,
    glm::vec3 startUp,
    GLfloat startYaw,
    GLfloat startPitch,
    GLfloat startMoveSpeed,
    GLfloat startTurnSpeed)
{
  position = startPosition;

  worldUp = startUp;
  yaw = startYaw;
  pitch = startPitch;
  front = glm::vec3(0.f, 0.f, -1.0f);

  moveSpeed = startMoveSpeed;
  turnSpeed = startTurnSpeed;

  update();
}

void Camera::update()
{
  printf("pitch: %.6f; yaw: %.6f \n", pitch, yaw);

  // х координата зависит от поворота по обоим осям
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  // y координата зависит только от поворота по у
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  // право - перпендикулярно к фронту и верху мира
  right = glm::normalize(glm::cross(front, worldUp));
  // верх камеры перпендикулярен фронту и праву
  up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool *keys, GLfloat deltaTime)
{
  GLfloat velocity = moveSpeed * deltaTime;

  if (keys[GLFW_KEY_W])
  {
    position += front * velocity;
  }

  if (keys[GLFW_KEY_S])
  {
    position -= front * velocity;
  }

  if (keys[GLFW_KEY_D])
  {
    position += right * velocity;
  }

  if (keys[GLFW_KEY_A])
  {
    position -= right * velocity;
  }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {

  // обновляем углы -> обновляются front, right, up -> движение идет по другим направлениям
  xChange *= turnSpeed;
  yChange *= turnSpeed;

  yaw += xChange;
  pitch += yChange;


  if(pitch > 89.f) {
    pitch = 89.f;
  }

  if(pitch < -89.f) {
    pitch = -89.f;
  }

  update();
}

glm::mat4 Camera::calculateViewMatrix()
{
  return glm::lookAt(position, position + front, up);
}

Camera::~Camera()
{
}

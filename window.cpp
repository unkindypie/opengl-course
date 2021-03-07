#include "window.h"

Window::Window()
{
  width = 800;
  height = 600;
  xChange = 0.f;
  yChange = 0.f;

  for (size_t i = 0; i < 1024; i++)
  {
    keys[i] = 0;
  }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
  width = windowWidth;
  height = windowHeight;
  xChange = 0.f;
  yChange = 0.f;

  for (size_t i = 0; i < 1024; i++)
  {
    keys[i] = 0;
  }
}

int Window::initialize()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    printf("GLFW initialisation failed!");
    glfwTerminate();
    return 1;
  }

  // Setup GLFW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core Profile = No Backwards Compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow Forward Compatbility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create the window
  mainWindow = glfwCreateWindow(width, height, "OpenGL be like", NULL, NULL);
  if (!mainWindow)
  {
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get Buffer Size information
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // handling key/mouse input
  createCallbacks();
  // прячу курсор, даже за пределами окна
  glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    printf("GLEW initialisation failed!");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }
  //включение проверки глубины(чтобы 3д вершины рисовались в правильном порядке)
  glEnable(GL_DEPTH_TEST);

  // Setup Viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  // вешаю референс этого объекта на референс glfw окна чтобы потом иметь возможность
  // юзать его внутри статик коллбека
  glfwSetWindowUserPointer(mainWindow, this);

  return 0;
}

void Window::createCallbacks()
{
  glfwSetKeyCallback(mainWindow, Window::handleKeys);
  glfwSetCursorPosCallback(mainWindow, Window::handleMouse);
}

GLfloat Window::getXChange()
{
  GLfloat temp = xChange;
  xChange = 0.f;
  return temp;
}

GLfloat Window::getYChange()
{
  GLfloat temp = yChange;
  yChange = 0.f;
  return temp;
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
  Window *owner = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024)
  {

    if (action == GLFW_PRESS)
    {
      owner->keys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
      owner->keys[key] = false;
    }
  }
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
  Window *owner = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (!owner->mouseFirstMoved)
  {
    owner->lastX = xPos;
    owner->lastY = yPos;
    owner->mouseFirstMoved = true;
  }
  owner->xChange = xPos - owner->lastX;
  // owner->yChange = yPos - owner->lastY;
  // наоборот т.к. верх и низ перевернуты
  owner->yChange = owner->lastY - yPos;

  owner->lastX = xPos;
  owner->lastY = yPos;

  printf("Mouse: x:%.6f, y:%.6f\n", owner->xChange, owner->yChange);
}

Window::~Window()
{
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}

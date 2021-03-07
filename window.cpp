#include "window.h"

Window::Window()
{
  printf("window def constructor");
  width = 800;
  height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
  printf("window param constructor");

  width = windowWidth;
  height = windowHeight;
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
}

Window::~Window()
{
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
//#define GLEW_STATIC
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "mesh.h"
#include "shader.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = M_PI / 180;
GLuint Shader, unifromModel, uniformProjection;

std::vector<Mesh*> meshlist;
class::Shader *shader = new class::Shader();
std::vector<class::Shader*> shaderlist;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 1.f;
float triIncement = 0.01f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

float curAngle = .0f;

//Vertex shader
static const char* vShader =
    "\n\
    #version 330\n\
    layout (location = 0) in vec3 pos;\n\
    uniform mat4 model;\n\
    uniform mat4 projection;\n\
    out vec4 vCol;//после вызова этого шейдера эта переменная будет передана в следующий шейдер\n\
    void main() {\n\
        gl_Position = projection * model * vec4(pos, 1.0);\n\
        vCol = vec4(clamp(pos, 0.0, 1.0), 1);\n\
    }";
//Fragment shader
static const char* fShader =
    "\n\
    #version 330\n\
    out vec4 color;\n\
    in vec4 vCol;\n\
    void main() {\n\
        color = vCol;//vec4(1.0, 0.5, 0.0, 1);\n\
    }";

void createObjects(){
    //индесксы для буфера индексов нужны для того, чтобы не
    //копипастить вершины по нескольку раз, т.к. они часто повторяются в 3д фигурах
    //по сути они ссылаются на вершины
    unsigned int indeces[] = {
      0, 3, 1, //1 сторона
      1, 3, 2, //2 сторона
      2, 3, 0, //3 сторона
      0, 1, 2 //дно (прямо как я)
    };

    GLfloat vertices[] = {
        -1.0f,  -1.0f, 0.0f,
         0, -1, 1,
         1.0f, -1.0f, 0.0f,
         0.0f, 1.0f, 0.0f
    };

    meshlist.push_back(new Mesh());
    meshlist[meshlist.size() - 1]->createMesh(vertices, indeces, 12, 12);

    meshlist.push_back(new Mesh());
    meshlist[meshlist.size() - 1]->createMesh(vertices, indeces, 12, 12);
}

void addShader(GLuint program, const char * shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    //т.к. glShaderSource может работать с несколькими строками, а у нас одна, то надо
    //загнать ее в массив(и длины строк тоже)
    const GLchar * code[1];

    code[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(code[0]);

    glShaderSource(theShader, 1, code, codeLength);
    glCompileShader(theShader);
    //обработка ошибок
    GLint result;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("error compiling %d shader :\n %s\n", shaderType, eLog);
        return;
    }
    glAttachShader(program, theShader);
}

void compileShaders(){
    //создание шейдера(возвращает id, сам шейдер лежит на видюхе)
    Shader = glCreateProgram();
    if(!Shader) {
        printf("error creating shader program");
        return;
    }
    addShader(Shader, vShader, GL_VERTEX_SHADER);
    addShader(Shader, fShader, GL_FRAGMENT_SHADER);
    GLint result;
    GLchar eLog[1024] = { 0 };

    //финальная линковка
    glLinkProgram(Shader);
    //обработка ошибок
    glGetProgramiv(Shader, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(Shader, sizeof(eLog), NULL, eLog);
        printf("error linking program:\n %s\n", eLog);
        return;
    }
    //валидация шейдера
    glValidateProgram(Shader);
    //обработка ошибок
    glGetProgramiv(Shader, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(Shader, sizeof(eLog), NULL, eLog);
        printf("error validating program:\n %s\n", eLog);
        return;
    }
    //получаю место в коде glsl шейдера, где создана юниформ переменная
    unifromModel = glGetUniformLocation(Shader, "model");
    uniformProjection = glGetUniformLocation(Shader, "projection");
}

void createShaders(){
    class::Shader *shader1 = new class::Shader();
    shader1->createFromString(vShader, fShader);
    shaderlist.push_back(shader1);
}

void printMat(glm::mat4 matrix) {
    std::string str = glm::to_string(matrix);
    size_t pos;
    int skips = 0;
    do {
        pos = str.find_first_of(',');

        if(pos > str.size()) break;

        if(skips == 3) {
            str.replace(pos, 1, "\n");
            skips = 0;
        }
        else {
            str.replace(pos, 1, " ");
            skips++;
        }
    } while(pos);

    std::cout << str << std::endl;
}

int main(int argc, char *argv[])
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
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size information
    int bufferWidth, bufferHeight;
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

    createObjects();
    createShaders();

    glm::mat4 projection = glm::perspective(45.f, (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.f);


    glm::mat4 _model(1); //конструктор еденичной матрицы
    //_model = glm::translate(_model, glm::vec3(triOffset, 0.0f, 0.0f));
    _model = glm::rotate(_model, 89 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    printMat(_model);

    glm::mat4 __model(1); //конструктор еденичной матрицы
    __model = glm::rotate(__model, 89 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
     __model = glm::translate(__model, glm::vec3(2, 5.5f, 0.0f));
    printMat(__model);

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + Handle user input events
        glfwPollEvents();

        if(direction){
            triOffset += triIncement;
        }
        else {
            triOffset -= triIncement;
        }
        if(std::abs(triOffset) >= triMaxOffset){
            direction = !direction;
            glm::mat4 model(1); //конструктор еденичной матрицы
            model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
            printMat(model);
        }
        curAngle += 0.5f;
        if(curAngle >= 360){
            curAngle = 0;
        }

        if(sizeDirection){
            curSize += 0.001;
        }
        else {
            curSize -= 0.001;
        }
        if(curSize > maxSize || curSize < minSize){
            sizeDirection = !sizeDirection;
        }

        // Clear window
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //говорю, к какому шейдеру сейчас отправлять вызовы всех шейдерных функций
        shaderlist[0]->useShader();

        //создаю матрицу преобразований
        glm::mat4 model(1); //конструктор еденичной матрицы
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
        //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0, 1, 0));
        //model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        //задаю юниформ матрицы преобразований, в который отправляю указатель на матрицу преобразований
        glUniformMatrix4fv(shaderlist[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        //юниформ матрицы проэкции
        glUniformMatrix4fv(shaderlist[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        meshlist[0]->renderMesh();

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-triOffset, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(unifromModel, 1, GL_FALSE, glm::value_ptr(model));
        meshlist[1]->renderMesh();

        glUseProgram(0);//отключаю шейдер

        glfwSwapBuffers(mainWindow);

    }

    return 0;
}

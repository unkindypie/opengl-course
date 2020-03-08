//#define GLEW_STATIC
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader, unifromModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncement = 0.005f;

//Vertex shader
static const char* vShader =
    "\n\
    #version 330\n\
    layout (location = 0) in vec3 pos;\n\
    uniform mat4 model;\n\
    void main() {\n\
        gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);\n\
    }";
//Fragment shader
static const char* fShader =
    "\n\
    #version 330\n\
    out vec4 color;\n\
    void main() {\n\
        color = vec4(1.0, 0.5, 0.0, 1);\n\
    }";

void createTriagnle(){
    GLfloat vertices[] = {
        -1.0f,  -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f, 1.0f, 0.0f
    };
    //генерирует айдишники для массива вертексов
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        //создает буфер для этого массива вертексов в памяти видюхи. Биндится он к самому последнему созданному VAO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            //атрибуты, которые говорят opengl что такое этот массив верктесов
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0); //включаю его

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); //0 - дальнейшие операции с вертксными буферами уже не будут биндится к этому буферу

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
    shader = glCreateProgram();
    if(!shader) {
        printf("error creating shader program");
        return;
    }
    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);
    GLint result;
    GLchar eLog[1024] = { 0 };

    //финальная линковка
    glLinkProgram(shader);
    //обработка ошибок
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("error linking program:\n %s\n", eLog);
        return;
    }
    //валидация шейдера
    glValidateProgram(shader);
    //обработка ошибок
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("error validating program:\n %s\n", eLog);
        return;
    }
    //получаю место в коде glsl шейдера, где создана юниформ переменная
    unifromModel = glGetUniformLocation(shader, "model");
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

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    createTriagnle();
    compileShaders();

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
        }

        // Clear window
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //говорю, к какому шейдеру сейчас отправлять вызовы всех шейдерных функций
        glUseProgram(shader);

        //создаю матрицу преобразований
        glm::mat4 model(1); //конструктор еденичной матрицы
        model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));

//        glUniform1f(uniformXMove, triOffset); //задаю юниформ шейдера
        //задаю юниформ матрицы преобразований, в который отправляю указатель на матрицу преобразований
        glUniformMatrix4fv(unifromModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);//отключаю шейдер

        glfwSwapBuffers(mainWindow);

    }

    return 0;
}

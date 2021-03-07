#include "shader.h"


Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode)
{
    compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char * vertexPath, const char * fragmentPath) {
    std::string vertexString = readFile(vertexPath);
    std::string fragmentString = readFile(fragmentPath);

    compileShader(vertexString.c_str(), fragmentString.c_str());
}

std::string Shader::readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        printf("Failed to open the file: %s\n", filePath);
        return "";
    }
    std::string fileLine = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, fileLine);
        content.append(fileLine + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode)
{
    //создание шейдера(возвращает id, сам шейдер лежит на видюхе)
    shaderID = glCreateProgram();
    if (!shaderID)
    {
        printf("error creating shader program");
        return;
    }
    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
    GLint result;
    GLchar eLog[1024] = {0};

    //финальная линковка
    glLinkProgram(shaderID);
    //обработка ошибок
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("error linking program:\n %s\n", eLog);
        return;
    }
    //валидация шейдера
    glValidateProgram(shaderID);
    //обработка ошибок
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("error validating program:\n %s\n", eLog);
        return;
    }
    //получаю место в коде glsl шейдера, где создана юниформ переменная
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    //т.к. glShaderSource может работать с несколькими строками, а у нас одна, то надо
    //загнать ее в массив(и длины строк тоже)
    const GLchar *code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(code[0]);

    glShaderSource(theShader, 1, code, codeLength);
    glCompileShader(theShader);
    //обработка ошибок
    GLint result;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("error compiling %d shader :\n %s\n", shaderType, eLog);
        return;
    }
    glAttachShader(program, theShader);
}

void Shader::useShader()
{
    glUseProgram(shaderID);
}

void Shader::clearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

Shader::~Shader()
{
    clearShader();
}

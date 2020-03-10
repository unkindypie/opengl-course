#include "shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode){
    compileShader(vertexCode, fragmentCode);
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode){
    //создание шейдера(возвращает id, сам шейдер лежит на видюхе)
    shaderID = glCreateProgram();
    if(!shaderID) {
        printf("error creating shader program");
        return;
    }
    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
    GLint result;
    GLchar eLog[1024] = { 0 };

    //финальная линковка
    glLinkProgram(shaderID);
    //обработка ошибок
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("error linking program:\n %s\n", eLog);
        return;
    }
    //валидация шейдера
    glValidateProgram(shaderID);
    //обработка ошибок
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("error validating program:\n %s\n", eLog);
        return;
    }
    //получаю место в коде glsl шейдера, где создана юниформ переменная
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
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

void Shader::useShader(){
    glUseProgram(shaderID);
}

void Shader::clearShader(){
    if(shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

GLuint Shader::GetModelLocation(){
    return uniformModel;
}

GLuint Shader::GetProjectionLocation(){
    return uniformProjection;
}

Shader::~Shader()
{
    clearShader();
}

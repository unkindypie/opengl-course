#ifndef SHADER_H
#define SHADER_H


#include <iostream>
#include <string.h>
#include <filesystem>

#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void createFromString(const char * vertexCode, const char * fragmentCode);
    void createFromFiles(const char * vertexPath, const char * fragmentPath);

    std::string readFile(const char* filePath);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();

    void useShader();
    void clearShader();

    ~Shader();
private:
    GLuint shaderID, uniformProjection, uniformModel;

    void compileShader(const char * vertexCode, const char * fragmentCode);
    void addShader(GLuint program, const char * shaderCode, GLenum shaderType);
};

#endif // SHADER_H

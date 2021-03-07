#version 330

// location = N - где N это индекс VAO буффера(в mesh.cpp он стоит в 0 в glVertexAttribPointer)
layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 projection;
out vec4 vCol;//после вызова этого шейдера эта переменная будет передана в следующий шейдер

void main() {
    gl_Position = projection * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0, 1.0), 1);
}
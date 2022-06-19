#version 330

// location = N - где N это индекс VAO буффера(в mesh.cpp он стоит в 0 в glVertexAttribPointer)
layout (location = 0) in vec3 pos; // позиция вертекста из атрибутов
layout (location = 1) in vec2 tex; // текстура из атрибутов


uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 vCol;//после вызова этого шейдера эта переменная будет передана в следующий шейдер
out vec2 texCoord;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0, 1.0), 1);

    texCoord = tex;
}

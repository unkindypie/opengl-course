#version 330

out vec4 color;
in vec4 vCol;
in vec2 texCoord;

uniform sampler2D theTexture;

void main() {
    // color = vCol;
    color = texture(theTexture, texCoord) * vCol;
    // color = texture(theTexture, texCoord);
}

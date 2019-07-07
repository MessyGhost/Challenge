#version 330 core

layout (location = 0) out vec4 FragColor;
in vec3 TexCoord;

uniform sampler2DArray texture2DArray;

void main() {
    FragColor = texture(texture2DArray, TexCoord);
}

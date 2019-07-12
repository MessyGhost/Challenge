#version 330 core

layout (location = 0) in vec3 ModelPosition;
layout (location = 1) in vec3 InTexCoord;
layout (location = 2) in vec3 Position;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 TexCoord;

void main() {
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(/*Position + */ModelPosition, 1.0);
    TexCoord = InTexCoord;
}

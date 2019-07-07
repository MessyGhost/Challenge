#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main() {
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(Position, 1.0);
}

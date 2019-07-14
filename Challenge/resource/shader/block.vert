#version 330 core

layout (location = 0) in vec3 ModelPosition;
layout (location = 1) in vec3 InTexCoord;
layout (location = 2) in vec3 PositionInChunk;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform vec3 ChunkPosition;

out vec3 TexCoord;

void main() {
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(ChunkPosition + PositionInChunk + ModelPosition, 1.0);
    TexCoord = InTexCoord;
}

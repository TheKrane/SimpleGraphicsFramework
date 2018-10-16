#version 330

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;

out vec3 FragPosition;
out vec2 FragUV;
out vec3 FragNormal;

void main()
{
    FragPosition = Position;
    FragUV = UV;
    FragNormal = Normal;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
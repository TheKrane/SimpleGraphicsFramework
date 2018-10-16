#version 330

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float Time;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;

out vec3 FragPosition;
out vec2 FragUV;
out vec3 FragNormal;

vec3 getWavePoint(float time, float x, float z)
{
    return vec3(Position.x, sin(time + x*2.0f + z*2.0f) * 0.1f, Position.z);
}

void main()
{
    FragPosition = Position;
    FragUV = UV;
    FragNormal = Normal;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(getWavePoint(Time, Position.x, Position.z), 1.0);
}
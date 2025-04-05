#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
    uniform mat4 viewProjection;
    uniform vec3 position;
};

out VS_OUT
{
    vec2 texCoord;
} vs_out;

void main()
{
    vs_out.texCoord = a_TexCoord;
    gl_Position = viewProjection * vec4(a_Position, 1.0);
}
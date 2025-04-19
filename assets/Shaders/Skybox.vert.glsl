#version 460 core
layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 cameraPosition;
};

out VS_OUT
{
    vec3 TexCoord;
} vs_out;

void main()
{
    vs_out.TexCoord = a_Position;
    vec4 pos = projection * view * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 cameraPosition;
};

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform mat4 u_model;

void main()
{
    vs_out.position = vec3(u_model * vec4(a_Position, 1.0));
    vs_out.normal = mat3(transpose(inverse(u_model))) * a_Normal;
    vs_out.texCoord = a_TexCoord;
    gl_Position = projection * view * u_model * vec4(a_Position, 1.0);
}
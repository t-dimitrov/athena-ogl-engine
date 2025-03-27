#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec2 a_UV;

layout(std140, binding = 0) uniform Camera
{
    uniform mat4 viewProjection;
};
uniform mat4 u_model;

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    mat3 TBN; // Tangent-Binormal-Normal matrix
} vs_out;

void main()
{
    vs_out.position = (u_model * vec4(a_Position, 1.0)).xyz;
    vs_out.normal = a_Normal;
    vs_out.uv = a_UV;

    vec3 bitangent = cross(a_Normal, a_Tangent.xyz) * a_Tangent.w;

    vec3 T = vec3(u_model * vec4(a_Tangent.xyz, 0.0));
    vec3 B = vec3(u_model * vec4(bitangent, 0.0));
    vec3 N = vec3(u_model * vec4(a_Normal, 0.0));
    vs_out.TBN = mat3(T, B, N);

    gl_Position = viewProjection * vec4(vs_out.position, 1.0);
}

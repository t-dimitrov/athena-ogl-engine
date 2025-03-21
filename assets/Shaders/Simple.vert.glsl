#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec2 a_UV;

uniform mat4 u_projection;
uniform mat4 u_view;
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
    vs_out.position = a_Position;
    vs_out.normal = a_Normal;
    vs_out.uv = a_UV;

    vec3 bitangent = cross(a_Normal, a_Tangent.xyz) * a_Tangent.w;

    vec3 T = normalize(vec3(u_model * a_Tangent));
    vec3 B = normalize(vec3(u_model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(u_model * vec4(a_Normal, 0.0)));
    vs_out.TBN = mat3(T, B, N);

    gl_Position = u_projection * u_view * u_model * vec4(a_Position, 1.0);
}

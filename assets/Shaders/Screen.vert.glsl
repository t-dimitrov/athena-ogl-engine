#version 460 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out VS_OUT
{
    vec2 TexCoord;
} vs_out;

void main()
{
    vs_out.TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
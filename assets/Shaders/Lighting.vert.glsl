#version 460 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out VS_OUT
{
    vec2 texCoord;
} vs_out;

void main()
{
    vs_out.texCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
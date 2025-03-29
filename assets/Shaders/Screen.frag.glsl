#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

uniform sampler2D colorTexture;

void main()
{
    FragColor = texture(colorTexture, fs_in.TexCoord);
}
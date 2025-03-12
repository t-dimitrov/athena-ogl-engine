#version 460 core
out vec4 FragColor;

struct Vertex
{
    vec4 color;
};
in Vertex vertex;

void main()
{
    FragColor = vertex.color;
}

#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

struct Vertex
{
    vec4 color;
};

out Vertex vertex;

void main()
{
    vertex.color = a_Color;
    gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}

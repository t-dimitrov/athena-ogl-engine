#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_projection;
uniform mat4 u_view;

struct Vertex
{
    vec4 color;
};

out Vertex vertex;

void main()
{
    vertex.color = a_color;
    gl_Position = u_projection * u_view * vec4(a_position.x, a_position.y, a_position.z, 1.0);
}

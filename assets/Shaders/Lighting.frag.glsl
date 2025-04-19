#version 460 core
out vec4 FragColor;

layout(std140, binding = 0) uniform Camera
{
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 cameraPosition;
};

in VS_OUT
{
    vec2 texCoord;
} fs_in;

struct Light
{
    vec3 position;
    vec3 color;
};
uniform Light u_light;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
    // Retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, fs_in.texCoord).xyz;
    vec3 normal = normalize(texture(gNormal, fs_in.texCoord).xyz);
    vec3 albedo = texture(gAlbedoSpec, fs_in.texCoord).rgb;

    vec3 ambient = albedo * 0.1f; // Ambient light

    // Lighting calculations
    vec3 lightDir = normalize(u_light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(cameraPosition - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

    vec3 diffuse = diff * u_light.color * albedo;
    vec3 specular = spec * u_light.color;

    FragColor = vec4(ambient + diffuse, 1.0);
}

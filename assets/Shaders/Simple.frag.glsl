#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    mat3 TBN; // Tangent-Binormal-Normal matrix
} fs_in;

struct Material
{
    sampler2D albedoTexture;
    sampler2D normalTexture;
};
uniform Material u_material;

struct Light
{
    vec3 direction;
    vec3 color;
};
uniform Light light;

void main()
{
    vec3 albedo = texture(u_material.albedoTexture, fs_in.uv).rgb;
    vec3 normal = texture(u_material.normalTexture, fs_in.uv).rgb;

    // Normal map values are usually in [0, 1], so remap to [-1, 1]
    normal = normalize(normal * 2.0 - 1.0); // Transform to tangent space normal
    //normal = normalize(fs_in.TBN * normal); // Using TBN matrix to convert to world space

    //// Directional light
    vec3 lightDir = normalize(-light.direction); // Light direction
    float diff = max(dot(normal, lightDir), 0.0); // Diffuse term
    vec3 diffuse = diff * light.color;

    FragColor = vec4((diffuse /*+ambient*/)*albedo, 1.0);
}

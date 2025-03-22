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
    float ambientStrength;
    vec3 color;
};
uniform Light light;

void main()
{
    vec3 albedo = texture(u_material.albedoTexture, fs_in.uv).rgb;
    vec3 normal = texture(u_material.normalTexture, fs_in.uv).rgb;
    
    // Normal Mapping
    normal = normal * 2.0 - 1.0;
    normal = normalize(fs_in.TBN * normal);

    // Directional light
    vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambientStrength * light.color;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    FragColor = vec4((ambient + diffuse) * albedo, 1.0);
}

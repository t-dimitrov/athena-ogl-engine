#version 460 core
layout(location = 0) out vec4 FragColor;        // Position data
layout(location = 1) out vec4 FragNormal;       // Normal data
layout(location = 2) out vec4 FragAlbedoSpec;   // Albedo and specular data

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fs_in;

struct Material
{
    sampler2D albedoTexture;
    int hasAlbedo;
    sampler2D normalTexture;
    int hasNormal;
};
uniform Material u_material;

void main()
{
    // Output the position to the first attachment
    FragColor = vec4(fs_in.position, 1.0);

    // Output the normal to the second attachment
    FragNormal = vec4(normalize(fs_in.normal), 1.0);

    // Output the albedo and specular to the third attachment
    FragAlbedoSpec = texture(u_material.albedoTexture, fs_in.texCoord);
}
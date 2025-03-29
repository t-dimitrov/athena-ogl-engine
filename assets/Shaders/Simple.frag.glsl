#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    mat3 TBN; // Tangent-Binormal-Normal matrix
    vec4 fragPosLight;
} fs_in;

struct Material
{
    sampler2D albedoTexture;
    int hasAlbedo;
    sampler2D normalTexture;
    int hasNormal;
};
uniform Material u_material;

uniform sampler2D u_shadowMap;

struct Light
{
    vec3 direction;
    float ambientStrength;
    vec3 color;
    mat4 projection;
};
uniform Light light;

void main()
{
    vec3 albedo = vec3(1.0, 1.0, 1.0);
    if (u_material.hasAlbedo > 0)
        albedo = texture(u_material.albedoTexture, fs_in.uv).rgb;

    vec3 normal = fs_in.normal;
    if (u_material.hasNormal > 0)
        normal = texture(u_material.normalTexture, fs_in.uv).rgb;
    
    // Normal Mapping
    if (u_material.hasNormal > 0)
    {
        normal = normal * 2.0 - 1.0;
        normal = normalize(fs_in.TBN * normal);
    }

    // Directional light
    vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambientStrength * light.color;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // Shadow
    float shadow = 0.0f;
    vec3 lightCoords = fs_in.fragPosLight.xyz / fs_in.fragPosLight.w;
    if (lightCoords.z <= 1.0)
    {
        lightCoords = (lightCoords + 1.0f) / 2.0f;

        float closestDepth = texture(u_shadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        float bias = max(0.025f * (1.0 - dot(fs_in.normal, lightDir)), 0.0005f);
        
        int sampleRadius = 4;
        vec2 pixelSize = 1.0 / textureSize(u_shadowMap, 0);
        for (int y = -sampleRadius; y <= sampleRadius; ++y)
        {
            for (int x = -sampleRadius; x <= sampleRadius; ++x)
            {
                float closestDepth = texture(u_shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                {
                    shadow += 1.0f;
                }
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    FragColor = vec4((ambient + diffuse * (1.0 - shadow)) * albedo, 1.0);
}

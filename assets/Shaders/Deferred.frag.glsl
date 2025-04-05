#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec2 texCoord;
} fs_in;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
    vec3 position;
    vec3 color;
};
const int LIGHTS_COUNT = 32;
uniform Light lights[LIGHTS_COUNT];

uniform vec3 viewPos;

void main()
{
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, fs_in.texCoord).rgb;
    vec3 Normal = texture(gNormal, fs_in.texCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, fs_in.texCoord).rgb;
    float Specular = texture(gAlbedoSpec, fs_in.texCoord).a;

    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    for (int i = 0; i < LIGHTS_COUNT; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].color;
        lighting += diffuse;
    }

    FragColor = vec4(lighting, 1.0);
}
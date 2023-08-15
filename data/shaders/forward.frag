#version 330 core

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ambient;

    sampler2D texture_albedo;
    bool use_albedo_map;

    sampler2D texture_metallic;
    bool use_metallic_map;

    sampler2D texture_roughness;
    bool use_roughness_map;

    sampler2D texture_ao;
    bool use_ao_map;

    bool is_glossy_rough;

    sampler2D texture_normal;
    bool use_normal_map;

    sampler2D texture_bump;
    float bump_scale;
    bool use_bump_map;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;

layout (location = 0) out vec3 FragOut;
layout (location = 1) out vec3 SaturationOut;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 camPos;

float toon_factors[4] = float[](0.1, 0.4, 0.7, 1.0);

float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}

void main() {
    vec4 albedo = material.use_albedo_map ? texture(material.texture_albedo, TexCoords) : vec4(material.albedo, 1.0);

    vec3 N = Normal;
    if (material.use_normal_map) {
        N = texture(material.texture_normal, TexCoords).rgb;
        N = N * 2.0 - 1.0;
        N = normalize(TBN * N); // normals in world space
    }
    if (material.use_bump_map) {
        // adapted from https://www.shadertoy.com/view/MsScRt
        vec2 texelSize = 1. / textureSize(material.texture_bump, 0);
        float H = texture(material.texture_bump, TexCoords).r;
        float Hx = texture(material.texture_bump, TexCoords + dFdx(TexCoords.xy)).r;
        float Hy = texture(material.texture_bump, TexCoords + dFdy(TexCoords.xy)).r;
        vec2 dxy = H - vec2(Hx, Hy);

        vec3 bump = normalize(vec3(dxy * material.bump_scale / texelSize, 1.0));
        bump = normalize(bump * 0.5 + 0.5);
        N = normalize(TBN * bump);
    }

    vec3 lightColor = dirLight.color * dirLight.intensity;
    vec3 combColor = lightColor * albedo.rgb;
    float NdotL = max(dot(-dirLight.direction, N), 0);
    vec4 thresholds = vec4(0.25, 0.5, 0.95, 1);
    vec4 res = step(NdotL, thresholds);
    int layer = 3 - int(res.x + res.y + res.z + res.w);
    vec3 outColor = combColor * toon_factors[layer];
    FragOut = outColor;

    // TODO: temporary, mock position and radius of color source
    vec3 srcPos = vec3(0, 0.5, 1.2);
    float srcR = 100;
    float distToSrc = distance(srcPos, FragPos);
    float satFactor = clamp(inverseLerp(distToSrc, srcR + 0.5, srcR), 0, 1);
    SaturationOut = vec3(satFactor);
}
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

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}

void main() {
    vec3 albedo = material.use_albedo_map ? texture(material.texture_albedo, TexCoords).rgb : material.albedo;
    // expect for packed textures to use ao (R) - roughness (G) - metallic (B) ordering
    float ao = material.use_ao_map ? texture(material.texture_ao, TexCoords).r : material.ambient;
    float roughness = material.use_roughness_map ? texture(material.texture_roughness, TexCoords).g : material.roughness;
    roughness = material.is_glossy_rough ? 1 - roughness : roughness;
    float metallic = material.use_metallic_map ? texture(material.texture_metallic, TexCoords).b : material.metallic;

    vec3 N = Normal;
    vec3 V = normalize(camPos - FragPos);
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

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // Directional light
    {
        vec3 L = normalize(-dirLight.direction);
        vec3 H = normalize(V + L);
        // skip attenuation
        vec3 radiance = dirLight.color * dirLight.intensity;

        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    FragOut = color;

    // TODO: temporary, mock position and radius of color source
    vec3 srcPos = vec3(0, 0.5, 1.2);
    float srcR = 100;
    float distToSrc = distance(srcPos, FragPos);
    float satFactor = clamp(inverseLerp(distToSrc, srcR + 0.5, srcR), 0, 1);
    SaturationOut = vec3(satFactor);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

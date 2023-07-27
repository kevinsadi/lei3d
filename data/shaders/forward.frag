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

float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}

void main() {
    vec4 albedo = material.use_albedo_map ? texture(material.texture_albedo, TexCoords) : vec4(material.albedo, 1.0);
//    vec4 albedo = texture(material.texture_albedo, TexCoords);

    vec3 N = Normal;
//    if (material.use_normal_map) {
//        N = texture(material.texture_normal, TexCoords).rgb;
//        N = Normal * 2.0 - 1.0;
//        N = normalize(TBN * N); // normals in world space
//    }
//    if (material.use_bump_map) {
//        // adapted from https://www.shadertoy.com/view/MsScRt
//        vec2 texelSize = 1. / textureSize(material.texture_bump, 0);
//        float H = texture(material.texture_bump, TexCoords).r;
//        float Hx = texture(material.texture_bump, TexCoords + dFdx(TexCoords.xy)).r;
//        float Hy = texture(material.texture_bump, TexCoords + dFdy(TexCoords.xy)).r;
//        vec2 dxy = H - vec2(Hx, Hy);
//
//        vec3 bump = normalize(vec3(dxy * material.bump_scale / texelSize, 1.0));
//        bump = normalize(bump * 0.5 + 0.5);
//        N = normalize(TBN * bump);
//    }

    vec3 lightColor = dirLight.color * dirLight.intensity;
    vec3 outColor = lightColor * albedo.rgb;
    float NdotL = dot(dirLight.direction, N);
    if (NdotL > 0.95) {
        outColor = outColor * 1.0;
    } else if (NdotL > 0.5) {
        outColor = outColor * 0.75;
    } else if (NdotL > 0.25) {
        outColor = outColor * 0.4;
    } else {
        outColor = outColor * 0.1;
    }
    FragOut = outColor;

    // TODO: temporary, mock position and radius of color source
    vec3 srcPos = vec3(0, 0.5, 1.2);
    float srcR = 1.5;
    float distToSrc = distance(srcPos, FragPos);
    float satFactor = clamp(inverseLerp(distToSrc, srcR + 0.5, srcR), 0, 1);
    SaturationOut = vec3(satFactor);
}
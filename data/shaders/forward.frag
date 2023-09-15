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

    float nearPlane;
    float farPlane;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;
in vec4 FragPos_lS;

layout (location = 0) out vec3 FragOut;
layout (location = 1) out vec3 SaturationOut;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 camPos;
uniform sampler2D shadowDepth;

const float PI = 3.14159265359;
const float PositiveExponent = 40.0;
const float NegativeExponent = 8.0;
const float lightBleedReduction = 0.1;
const int NUM_PCF_SAMPLES = 32;
const vec2 Poisson[32] = vec2[](
    vec2(-0.975402, -0.0711386),
    vec2(-0.920347, -0.41142),
    vec2(-0.883908, 0.217872),
    vec2(-0.884518, 0.568041),
    vec2(-0.811945, 0.90521),
    vec2(-0.792474, -0.779962),
    vec2(-0.614856, 0.386578),
    vec2(-0.580859, -0.208777),
    vec2(-0.53795, 0.716666),
    vec2(-0.515427, 0.0899991),
    vec2(-0.454634, -0.707938),
    vec2(-0.420942, 0.991272),
    vec2(-0.261147, 0.588488),
    vec2(-0.211219, 0.114841),
    vec2(-0.146336, -0.259194),
    vec2(-0.139439, -0.888668),
    vec2(0.0116886, 0.326395),
    vec2(0.0380566, 0.625477),
    vec2(0.0625935, -0.50853),
    vec2(0.125584, 0.0469069),
    vec2(0.169469, -0.997253),
    vec2(0.320597, 0.291055),
    vec2(0.359172, -0.633717),
    vec2(0.435713, -0.250832),
    vec2(0.507797, -0.916562),
    vec2(0.545763, 0.730216),
    vec2(0.56859, 0.11655),
    vec2(0.743156, -0.505173),
    vec2(0.736442, -0.189734),
    vec2(0.843562, 0.357036),
    vec2(0.865413, 0.763726),
    vec2(0.872005, -0.927));

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}

vec3 srgb_to_linear(vec3 color) {
    // Approximation from http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
    return color * (color * (color * 0.305306011 + 0.682171111) + 0.012522878);
}

float calcShadowPCF(vec3 normal) {
    vec3 coords = FragPos_lS.xyz / FragPos_lS.w;
    coords = coords * 0.5 + 0.5;

    float bias = max(0.05 * (1.0 - dot(normal, -dirLight.direction)), 0.005);
    float closestDepth = texture(shadowDepth, coords.xy).x;
    float currDepth = coords.z;

    float shadow = 0.0;
    if (!(currDepth > 1.0)) {
        vec2 texelSize = vec2(1.0);
        texelSize /= textureSize(shadowDepth, 0);

        // PCF
        for (int i = 0; i < NUM_PCF_SAMPLES; i++) {
            float pcfDepth = 1.0;
            pcfDepth = texture(shadowDepth, coords.xy + Poisson[i] * texelSize).r;
            shadow += currDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
        shadow /= float(NUM_PCF_SAMPLES);
    }

    return shadow;
}

void main() {
    vec3 albedo = material.use_albedo_map ? texture(material.texture_albedo, TexCoords).rgb : material.albedo;
    albedo = srgb_to_linear(albedo);
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

    float visible = 1.0 - calcShadowPCF(N);
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo * visible;

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

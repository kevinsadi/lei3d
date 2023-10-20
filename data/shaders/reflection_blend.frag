// Adapted from: https://github.com/keptsecret/CivetRender/blob/master/src/shaders/reflection_frag.glsl
#version 330 core
out vec4 FragColor;

uniform sampler2D DepthMap;
uniform sampler2D AlbedoMap;
uniform sampler2D MetallicRoughnessMap;
uniform sampler2D NormalMap;
uniform sampler2D ReflectedMap;

uniform mat4 invView;
uniform mat4 invProjection;
uniform vec2 screenSize;
uniform vec3 camPos;

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 blur(sampler2D image, vec2 uv, float roughness);

vec2 getTexCoords() {
    return gl_FragCoord.xy / screenSize;
}

void main() {
    vec2 texCoords = getTexCoords();

    float depth = texture(DepthMap, texCoords).x;
    vec4 clipPos = vec4(texCoords.xy * 2.0 - 1.0, depth * 2.0 - 1.0, 1);
    vec4 worldPos = invView * invProjection * clipPos;
    worldPos /= worldPos.w;
    vec3 pos_wS = worldPos.xyz;

    float metallic = texture(MetallicRoughnessMap, texCoords).r;
    float roughness = texture(MetallicRoughnessMap, texCoords).g;
    float roughnessFade = smoothstep(0.3, 0.85, 1.0 - roughness);

//    vec3 reflectedColor = blur(ReflectedMap, texCoords, roughness);
    vec3 reflectedColor = texture(ReflectedMap, texCoords).rgb;

    vec3 albedo = texture(AlbedoMap, texCoords).rgb;
    vec3 F0 = mix(vec3(0.03), albedo, metallic);

    vec3 N = normalize(texture(NormalMap, texCoords).xyz);
    vec3 V = normalize(camPos - pos_wS);
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.001), F0, roughness);
    reflectedColor *= F * roughnessFade;

    FragColor.rgb = reflectedColor;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 blur(sampler2D image, vec2 texCoord, float roughness) {
    const float TwoPi = 6.28318530718;
    const float Directions = 16.0;
    const float Quality = 6.0;
    const float MaxSize = 64.0;

    float Size = smoothstep(1.0, 64.0, roughness);

    vec2 radius = Size / textureSize(image, 0);
    vec3 color = texture(image, texCoord).rgb;

    for(float d = 0.0; d < TwoPi; d += TwoPi / Directions) {
        for(float i = 1.0 / Quality; i < 1.001; i += 1.0 / Quality) {
            color += texture(image, texCoord + vec2(cos(d), sin(d)) * radius * i).rgb;
        }
    }

    color /= Quality * Directions + 1.0;
    return color;
}
#version 330 core
out vec4 FragOut;
out vec4 SaturationOut;

struct ColorSource {
    vec3 position;
    float radius;
    float falloff;
};

in vec3 TexCoords; // direction vector representing a 3D texture coordinate
uniform samplerCube skyboxCubemap; // cubemap texture sampler

uniform ColorSource colorSources[5];
uniform int numColorSources;

const float rangeStart = 400.0;

float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}

vec3 srgb_to_linear(vec3 color) {
    // Approximation from http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
    return color * (color * (color * 0.305306011 + 0.682171111) + 0.012522878);
}

void main()
{
    FragOut = texture(skyboxCubemap, TexCoords);
    FragOut.rgb = srgb_to_linear(FragOut.rgb);

    vec3 skyPos = rangeStart * normalize(TexCoords);
    float satFactor = 0.0;
    for (int i = 0; i < numColorSources; i++) {
        vec3 p = colorSources[i].position;
        float r = colorSources[i].radius;
        float distToSrc = distance(p, skyPos);
        satFactor += clamp(inverseLerp(distToSrc, r + colorSources[i].falloff, r), 0, 1);
    }
    satFactor = clamp(satFactor, 0, 1);

    SaturationOut.rgb = vec3(satFactor);
}
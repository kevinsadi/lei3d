#version 330 core

out vec4 FragColor;

uniform sampler2D RawFinalImage;
uniform sampler2D SaturationMask;

in vec2 TexCoords;

float linear_luminance(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

float gamma_luminance(vec3 color) {
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

vec3 desaturate(vec3 color, float factor) {
    vec3 gray = vec3(linear_luminance(color));
    return mix(gray, color, factor);
}

// ACES filmic tonemapping curve adapted to GLSL from: https://github.com/godotengine/godot/blob/master/drivers/gles3/shaders/tonemap_inc.glsl
vec3 aces_fitted(vec3 color) {
    const float exposure_bias = 1.8f;
    const float A = 0.0245786f;
    const float B = 0.000090537f;
    const float C = 0.983729f;
    const float D = 0.432951f;
    const float E = 0.238081f;

    // Exposure bias baked into transform to save shader instructions. Equivalent to `color *= exposure_bias`
    const mat3 rgb_to_rrt = mat3(
    vec3(0.59719f * exposure_bias, 0.35458f * exposure_bias, 0.04823f * exposure_bias),
    vec3(0.07600f * exposure_bias, 0.90834f * exposure_bias, 0.01566f * exposure_bias),
    vec3(0.02840f * exposure_bias, 0.13383f * exposure_bias, 0.83777f * exposure_bias));

    const mat3 odt_to_rgb = mat3(
    vec3(1.60475f, -0.53108f, -0.07367f),
    vec3(-0.10208f, 1.10813f, -0.00605f),
    vec3(-0.00327f, -0.07276f, 1.07602f));

    color *= rgb_to_rrt;
    vec3 color_tonemapped = (color * (color + A) - B) / (color * (C * color + D) + E);
    color_tonemapped *= odt_to_rgb;

    return clamp(color_tonemapped, 0.0, 1.0);
}

// Reinhard tonemapping variant by Jodie: https://www.shadertoy.com/view/4dBcD1
vec3 reinhard_jodie(vec3 color) {
    float lum = gamma_luminance(color);
    vec3 color_tonemapped = color / (color + 1.0);
    return mix(color / (lum + 1.0), color_tonemapped, color_tonemapped);
}

// Approximate conversion to srgb from http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
// Use instead of gamma correction
vec3 linear_to_srgb(vec3 color) {
    return max(vec3(1.055) * pow(color, vec3(0.416666667)) - vec3(0.055), vec3(0.0));
}

void main() {
    vec3 color = texture(RawFinalImage, TexCoords).rgb;
    float factor = texture(SaturationMask, TexCoords).r;
    vec3 finalColor = desaturate(color, factor);

    vec3 tonemapped = reinhard_jodie(finalColor);

    FragColor.rgb = linear_to_srgb(tonemapped);
}
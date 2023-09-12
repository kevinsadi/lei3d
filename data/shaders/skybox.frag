#version 330 core
out vec4 FragColor;

in vec3 TexCoords; // direction vector representing a 3D texture coordinate
uniform samplerCube skyboxCubemap; // cubemap texture sampler

vec3 srgb_to_linear(vec3 color) {
    // Approximation from http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
    return color * (color * (color * 0.305306011 + 0.682171111) + 0.012522878);
}

void main()
{
    FragColor = texture(skyboxCubemap, TexCoords);
    FragColor.rgb = srgb_to_linear(FragColor.rgb);
}
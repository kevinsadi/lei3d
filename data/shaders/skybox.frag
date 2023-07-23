#version 330 core
out vec4 FragColor;

in vec3 TexCoords; // direction vector representing a 3D texture coordinate
uniform samplerCube u_TexSkybox; // cubemap texture sampler

void main()
{
    FragColor = texture(u_TexSkybox, TexCoords);
}
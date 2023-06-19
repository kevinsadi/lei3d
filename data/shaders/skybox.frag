#version 330 core
out vec4 FragColor;

in vec3 TexCoords; // direction vector representing a 3D texture coordinate
uniform samplerCube skybox; // cubemap texture sampler

void main()
{
    FragColor = texture(skybox, TexCoords);
}
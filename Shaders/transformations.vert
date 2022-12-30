#version 330 core
layout (location = 0) in vec3 aPos; // read in position from buffer data
layout (location = 1) in vec3 vertColor; // read in color from buffer data
layout (location = 2) in vec2 aTexCoord;

out vec3 color; // for output to frag shader
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// outputting the vertex position as color to see what happens
void main()
{
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aPos;
    TexCoord = aTexCoord;
}
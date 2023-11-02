#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform vec4 color;

void main() 
{
    // everything should be normalized
    vertexColor = color;
    TexCoord = aTexCoord;

    vec2 position = aPosition;

    gl_Position = transform * vec4(position, 0, 1.0);

}
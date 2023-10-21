#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform vec2 screenSize;
uniform vec4 color;
uniform bool normalized;

void main() 
{
    vertexColor = color;
    TexCoord = aTexCoord;

    vec2 position = aPosition;

    if (!normalized) 
    {
        position = (vec2(aPosition.x / screenSize.x, aPosition.y / screenSize.y) * 2.0) - 1.0;
    }

    gl_Position = transform * vec4(position, 0, 1.0);

}
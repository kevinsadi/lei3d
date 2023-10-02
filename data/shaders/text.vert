#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform vec2 screenSize;

void main() 
{
    vertexColor = aColor;
    TexCoord = aTexCoord;

    vec2 normalizedPosition = (vec2(aPosition.x / screenSize.x, aPosition.y / screenSize.y) * 2.0) - 1.0;
    gl_Position = transform * vec4(normalizedPosition, 0, 1.0);
}
#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 translation;
uniform mat4 scale;

uniform vec4 color;

void main() 
{
    vertexColor = color;
    TexCoord = aTexCoord;

    // changing from (0, 0) in top left and (1, 1) in bottom right to
    // opengl coordinates, (-1, -1) in bottom left and (1, 1) in 
    // top right
    vec2 position = vec2(1.0, -1.0) * (aPosition * 2.0 - 1.0);

    gl_Position = translation * scale * vec4(position, 0, 1.0);

}
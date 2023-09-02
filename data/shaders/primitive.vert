#version 330 core
layout (location = 0) in vec3 aPos; // read in position from buffer data
//layout (location = 2) in vec2 aTexCoord; // read in texture coordinate from buffer data

//out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

// outputting the vertex position as color to see what happens
void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //TexCoord = aTexCoord;
}
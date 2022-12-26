#version 330 core
layout (location = 0) in vec3 aPos; // read in position from buffer data
layout (location = 1) in vec3 vertColor; // read in color from buffer data
out vec3 color; // for output to frag shader


// outputting the vertex position as color to see what happens
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aPos;
}
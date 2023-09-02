#version 330 core
out vec4 FragColor;

in vec3 color;
//in vec2 TexCoord;

uniform vec3 u_Color;

void main()
{
   FragColor = vec4(u_Color, 1);
   //FragColor = texture(ourTexture, TexCoord);
}

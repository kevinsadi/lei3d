#version 330 core

out vec2 TexCoords;

void main() {
    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    vec2 texCoord = (position + 1) * 0.5;

    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    TexCoords = (position + 1) * 0.5;
}
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 FragPos;

uniform vec3 lightPos;
uniform float lightNearPlane;
uniform float lightFarPlane;

const float PositiveExponent = 40.0;
const float NegativeExponent = 8.0;

void main() {
    vec3 dir = FragPos - lightPos;
    float distanceSquared = dot(dir, dir);
    float distance = (sqrt(distanceSquared) - lightNearPlane) / (lightFarPlane - lightNearPlane);

    vec4 moments = vec4(exp(PositiveExponent * distance), 0, -exp(-NegativeExponent * distance), 0);
    moments.yw = moments.xz * moments.xz;

    FragColor = moments;
}
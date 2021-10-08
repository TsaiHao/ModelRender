#version 330 core

in vec3 texCoord;
in vec3 vertNorm;

out vec4 glColor;

void main() {
    glColor = vec4(0.8f, 0.6f, 0.7f, 1.0f);
}
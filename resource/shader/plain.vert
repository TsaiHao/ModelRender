layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 tex;
layout (location = 2) in vec3 norm;

out vec3 texCoord;
out vec3 vertNorm;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * pos;
    texCoord = tex;
    vertNorm = norm;
}
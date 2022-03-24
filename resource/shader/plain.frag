in vec3 texCoord;
in vec3 vertNorm;

in vec4 fragPos;
out vec4 glColor;
uniform vec4 lightPos;

void main() {
    //glColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float l = length(lightPos - fragPos);
    glColor = vec4(l, l, l, 1.0f);
}
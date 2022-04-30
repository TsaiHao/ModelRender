in mediump vec3 texCoord;
in mediump vec3 vertNorm;

in mediump vec4 fragPos;
out mediump vec4 glColor;
uniform mediump vec4 lightPos;

void main() {
    glColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
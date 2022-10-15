in mediump vec3 texCoord;
in mediump vec3 vertNorm;

in mediump vec4 fragPos;
out mediump vec4 glColor;
uniform mediump sampler2D texture1;

void main() {
    glColor = texture(texture1, texCoord.xy);
}
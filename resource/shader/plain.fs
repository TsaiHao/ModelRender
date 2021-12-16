in vec3 texCoord;
in vec3 vertNorm;

out vec4 glColor;

uniform sampler2D texture1;

void main() {
    glColor = texture(texture1, texCoord.xy);
}
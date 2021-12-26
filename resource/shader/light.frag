out vec4 fragColor;

in vec3 texCoord;
in vec3 vertNorm;
in vec4 vertPos;

uniform sampler2D texture1;
uniform vec3 lightPos;

void main() {
    vec4 basicColor = vec4(1.0f, 0.5f, 0.3f, 1.0f);
    fragColor = basicColor;
}
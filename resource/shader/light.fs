out vec4 fragColor;

in vec3 texCoord;
in vec3 vertNorm;
in vec4 vertPos;
uniform vec3 lightPos;

void main() {
    vec4 basicColor = vec4(1.0f, 0.5f, 0.8f, 1.0f);
    vec3 dist = vertPos.xyz - lightPos;
    fragColor = vec4(dist, 1.0f);    
}
out vec4 fragColor;

in vec3 texCoord;
in vec3 vertNorm;
in vec4 vertPos;
in vec4 fragPos;

uniform vec4 lightPos;

void main() {
    float ambientStrength = 0.2f;
    vec4 basicColor = vec4(1.0f, 0.5f, 0.1f, 1.0f);

    vec3 vertNormDir = normalize(vertNorm);
    vec3 lightDir = normalize((lightPos - fragPos).xyz);

    vec3 ambientColor = ambientStrength * vec3(basicColor.xyz);
    vec3 diffuseColor = max(dot(vertNormDir, lightDir), 0) * vec3(basicColor.xyz);

    fragColor = vec4(ambientColor + diffuseColor, 1.0f);
}
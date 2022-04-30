layout (location = 0) in mediump vec4 pos;
layout (location = 1) in mediump vec3 tex;
layout (location = 2) in mediump vec3 norm;

out mediump vec3 texCoord;
out mediump vec3 vertNorm;
out mediump vec4 vertPos;
out mediump vec4 fragPos;

uniform mediump mat4 model;
uniform mediump mat4 view;
uniform mediump mat4 projection;

void main() {
    gl_Position = projection * view * model * pos;
    vertPos = gl_Position;
    texCoord = tex;
    vertNorm = (transpose(inverse(model)) * vec4(norm, 1.0f)).xyz;
    fragPos = model * pos;
}
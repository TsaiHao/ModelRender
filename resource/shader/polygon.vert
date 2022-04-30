layout (location = 0) in mediump vec3 pos;
layout (location = 1) in mediump vec2 tex;
out mediump vec3 texCoord;

void main() {
    gl_Position = vec4(pos, 1.0);
    texCoord = vec3(1.0f - tex.x, 1.0f - tex.y, 1.0f);
}

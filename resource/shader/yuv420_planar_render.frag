in vec3 texCoord;
in vec3 vertNorm;
in vec4 vertPos;
in vec4 fragPos;

// 0: RGB/RGBA/R 1: YUV420_Planar
uniform int pixelFormat = 0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

out vec4 fragColor;

vec4 yuv2rgba(float y, float u, float v) {
    float r = y +             1.402 * v;
    float g = y - 0.344 * u - 0.714 * v;
    float b = y + 1.772 * u;
    return vec4(r, g, b, 1.0f);
}

vec4 sampleTexture() {
    if (pixelFormat == 0) {
        return texture(texture1, texCoord.xy);
    } else if (pixelFormat == 1) {
        float y = texture(texture1, texCoord.xy).r;
        float u = texture(texture2, texCoord.xy).r - 0.5;
        float v = texture(texture3, texCoord.xy).r - 0.5;
        return yuv2rgba(y, u, v);
    }
    return vec4(0, 0, 0, 1.0f);
}

void main() {
    fragColor = sampleTexture();
}

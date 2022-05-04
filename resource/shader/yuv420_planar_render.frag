in mediump vec3 texCoord;
in mediump vec3 vertNorm;
in mediump vec4 vertPos;
in mediump vec4 fragPos;

// 0: RGB/RGBA/R 1: YUV420_Planar
uniform mediump int pixelFormat;
uniform mediump sampler2D texture1;
uniform mediump sampler2D texture2;
uniform mediump sampler2D texture3;

out mediump vec4 fragColor;

mediump vec4 yuv2rgba(mediump float y, mediump float u, mediump float v) {
    mediump float r = y +             1.402 * v;
    mediump float g = y - 0.344 * u - 0.714 * v;
    mediump float b = y + 1.772 * u;
    return vec4(r, g, b, 1.0f);
}

mediump vec4 sampleTexture() {
    if (pixelFormat == 0) {
        return texture(texture1, texCoord.xy);
    } else if (pixelFormat == 1) {
        mediump float y = texture(texture1, texCoord.xy).r;
        mediump float u = texture(texture2, texCoord.xy).r - 0.5;
        mediump float v = texture(texture3, texCoord.xy).r - 0.5;
        return yuv2rgba(y, u, v);
    }
    return vec4(0, 0, 0, 1.0f);
}

void main() {
    fragColor = sampleTexture();
}

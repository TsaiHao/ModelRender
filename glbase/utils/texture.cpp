#include "texture.h"

#include <utility>
#include "stb/stb_image.h"
#include "cgutils.h"

using namespace std;

std::pair<GLint, GLint> convert2GLFormat(ColorFormat fmt) {
    switch (fmt) {
        case ColorFormat::YUV420_Planar:
        case ColorFormat::YUV420_SemiPlanar:
        case ColorFormat::R8:
            return {GL_R8, GL_RED};
        case ColorFormat::RGB888:
            return {GL_RGB8, GL_RGB};
        case ColorFormat::RGBA8888:
            return {GL_RGBA8, GL_RGBA};
    }
}

TextureBuffer::TextureBuffer(unsigned char* data, int w, int h, ColorFormat fmt):
        buffer(data), format(fmt), width(w), height(h) {
    int64_t bufferSize = 0;
    switch (fmt) {
        case ColorFormat::YUV420_Planar:
        case ColorFormat::YUV420_SemiPlanar:
            bufferSize = w * h * 1.5; break;
        case ColorFormat::RGBA8888:
            bufferSize = w * h * 4; break;
        case ColorFormat::R8:
            bufferSize = w * h; break;
        case ColorFormat::RGB888:
            bufferSize = w * h * 3; break;
    }
}

const unsigned char* TextureBuffer::data() const {
    return buffer;
}

size_t TextureBuffer::size() const {
    return 0;
}

ImageTexture::ImageTexture() = default;

ImageTexture::ImageTexture(std::string img, int u): imagePath(std::move(img)), unit(u) {
}


void ImageTexture::setParam(GLenum type, GLint value) const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, type, value);
}

void ImageTexture::bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    _glCheckError();
}

void ImageTexture::bufferData(const unsigned char* data, int width, int height, ColorFormat format) const {
    bind();
    auto glFormat = convert2GLFormat(format);
    glTexImage2D(GL_TEXTURE_2D, 0, glFormat.first, width, height, 0, glFormat.second, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

ImageTexture::ImageTexture(const ImageTexture &tex) {
    texture = tex.texture;
    imagePath = tex.imagePath;
}

static GLuint genTexture(int unit, GLuint program, const char* textureUniform, int pixFormat) {
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(unit + GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, textureUniform), unit);
    glUniform1i(glGetUniformLocation(program, PIXEL_FORMAT), pixFormat);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void ImageTexture::init(GLuint currentProgram) {
    texture = genTexture(unit, currentProgram, TEXTURE_UNIFORM, 0);

    if (!imagePath.empty()) {
        int width, height, nChannel;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nChannel, 0);
        if (data) {
            ColorFormat format = ColorFormat::RGBA8888;
            if (nChannel == 3) {
                format = ColorFormat::RGB888;
            } else if (nChannel == 1) {
                format = ColorFormat::R8;
            }

            auto glFormat = convert2GLFormat(format);
            glTexImage2D(GL_TEXTURE_2D, 0, glFormat.first, width, height, 0, glFormat.second, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
}

GLuint ImageTexture::getTexture() const {
    return texture;
}


YUVTexture::YUVTexture(int u): unit1(u) {
}

void YUVTexture::setParam(GLenum type, GLint value) const {

}

GLuint YUVTexture::getTexture() const {
    return texture.front();
}

void YUVTexture::bind() const {
    glActiveTexture(GL_TEXTURE0 + unit1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glActiveTexture(GL_TEXTURE0 + unit1 + 1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glActiveTexture(GL_TEXTURE0 + unit1 + 2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
}

void YUVTexture::init(GLuint currentProgram) {
    GLuint tex = genTexture(unit1, currentProgram, TEXTURE_UNIFORM, 1);
    GLuint tex2 = genTexture(unit1 + 1, currentProgram, TEXTURE_UNIFORM2, 1);
    GLuint tex3 = genTexture(unit1 + 2, currentProgram, TEXTURE_UNIFORM3, 1);
    texture = {tex, tex2, tex3};
}

void YUVTexture::bufferData(const uint8_t* const* yuvData, int w, int h, ColorFormat fmt) {
    if (texture.empty()) {
        Logger::error("buffer data into uninitialized texture");
        return;
    }

    glActiveTexture(GL_TEXTURE0 + unit1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, yuvData[0]);

    glActiveTexture(GL_TEXTURE0 + unit1 + 1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w / 2, h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, yuvData[1]);

    glActiveTexture(GL_TEXTURE0 + unit1 + 2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w / 2, h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, yuvData[2]);
}

YUVTexture::~YUVTexture() = default;

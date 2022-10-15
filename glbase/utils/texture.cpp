#include "texture.h"

#include <utility>
#include <vector>
#include "stb/stb_image.h"
#include "cgutils.h"

using namespace std;

struct GLFormatTable {
    ColorFormat colorFormat;
    int nPlane;
    std::vector<GLint> glInternalFormat;
    std::vector<GLenum> glFormat;
};

static const std::vector<GLFormatTable> _ColorFormatQueryTable = {
    {ColorFormat::R8,       1, {GL_R8},                 {GL_RED}                },
    {ColorFormat::RGB888,   1, {GL_RGB8},               {GL_RGB}                },
    {ColorFormat::RGBA8888, 1, {GL_RGBA},               {GL_RGBA}               },
    {ColorFormat::YUV420P,  3, {GL_R8, GL_R8, GL_R8},   {GL_RED, GL_RED, GL_RED}},
    {ColorFormat::YUV420SP, 2, {GL_R8, GL_R8},          {GL_RED, GL_RG}         }
};

static inline GLFormatTable findFormat(ColorFormat format) {
    auto iter = std::find_if(_ColorFormatQueryTable.begin(), _ColorFormatQueryTable.end(), 
        [format](const GLFormatTable& ft) {
            return ft.colorFormat == format;
        });
    
    if (iter == _ColorFormatQueryTable.end()) {
        Logger::error("find format failed %d", (int)format);
        assert(false);
    }

    return *iter;
}

static inline GLuint genTexture(int unit) {
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(unit + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    /*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    */

    //glGenerateMipmap(GL_TEXTURE_2D);
    _glCheckError();

    return texture;
}

ImageTexture::ImageTexture(int width, int height, ColorFormat format) {
    auto glFormat = findFormat(format);

    for (int i = 0; i < glFormat.nPlane; ++i) {
        auto tid = genTexture(i);
        textureId.push_back(tid);
    }
}

void ImageTexture::setParam(GLenum type, GLint value) const {
    for (auto && id : textureId) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, type, value);
    }
}

void ImageTexture::bind() const {
    for (int i = 0; i < textureId.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureId[i]);
    }
    _glCheckError();
}

void ImageTexture::bufferData(const TextureBuffer &buffer) const {
    bind();
    auto glFormat = findFormat(buffer.format);

    for (int i = 0; i < textureId.size(); ++i) {
        glTexImage2D(GL_TEXTURE_2D, 
                        0, 
                        glFormat.glInternalFormat[i], 
                        buffer.width, 
                        buffer.height, 
                        0, 
                        glFormat.glFormat[i], 
                        GL_UNSIGNED_BYTE, 
                        buffer.buffer[i]);

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    _glCheckError();
}

std::shared_ptr<ImageTexture> createTexture(std::string_view imgPath) {
    int width, height, channel;

    unsigned char* data = stbi_load(imgPath.data(), &width, &height, &channel, 0);
    if (data == nullptr) {
        Logger::error("load image %s fail", imgPath.data());
        assert(false);
    }

    ColorFormat fmt = ColorFormat::RGB888;
    if (channel == 4) {
        fmt = ColorFormat::RGBA8888;
    } else if (channel == 1) {
        fmt = ColorFormat::R8;
    }

    auto tex = std::make_shared<ImageTexture>(width, height, fmt);
    TextureBuffer buffer({data}, width, height, fmt);
    tex->bufferData(buffer);

    return tex;
}

#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <string>
#include <vector>
#include <memory>
#include "cgdefs.h"
#include "cgutils.h"

enum class ColorFormat {
    YUV420P,
    YUV420SP,
    R8,
    RGB888,
    RGBA8888
};

class TextureBuffer {
public:
    TextureBuffer(std::vector<uint8_t*> data, int width, int height, ColorFormat fmt = ColorFormat::RGB888)
            : format(fmt)
            , width(width)
            , height(height)
            , buffer(std::move(data)) {

        }

    ColorFormat format;

    int width = 0;

    int height = 0;

    std::vector<uint8_t*> buffer;
};

class ImageTexture {
public:
    ImageTexture(int width, int height, ColorFormat format);

    ImageTexture(const ImageTexture& tex) = delete;

    ~ImageTexture() = default;

    void setParam(GLenum type, GLint value) const;

    void bind() const;

    void bufferData(const TextureBuffer &buffer) const;

private:
    std::vector<GLuint> textureId;
    std::string imagePath {};
};

std::shared_ptr<ImageTexture> createTexture(std::string_view imgPath);

#endif //GRAPHICS_TEXTURE_H

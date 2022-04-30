#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <string>
#include <vector>
#include "cgdefs.h"
#include "cgutils.h"

enum class ColorFormat {
    YUV420_Planar,
    YUV420_SemiPlanar,
    R8,
    RGB888,
    RGBA8888
};

std::pair<GLint, GLint> convert2GLFormat(ColorFormat fmt);

class TextureBuffer {
public:
    TextureBuffer(unsigned char* data, int width, int height, ColorFormat fmt = ColorFormat::YUV420_SemiPlanar);

    [[nodiscard]] const unsigned char* data() const;

    size_t size() const;

    ColorFormat format;

    int width = 0;

    int height = 0;

private:
    unsigned char* buffer;
};

class Texture {
public:
    virtual ~Texture() = default;

    virtual void init(GLuint currentProgram) = 0;

    virtual void setParam(GLenum type, GLint value) const = 0;

    virtual GLuint getTexture() const = 0;

    virtual void bind() const = 0;

};

class ImageTexture: public Texture {
public:
    ImageTexture();

    explicit ImageTexture(std::string imagePath, int unit = 0);

    ImageTexture(const ImageTexture& tex);

    ~ImageTexture() override = default;

    void init(GLuint currentProgram) override;

    void setParam(GLenum type, GLint value) const override;

    GLuint getTexture() const override;

    void bind() const override;

    void bufferData(const unsigned char* data, int width, int height, ColorFormat format) const;

private:
    GLuint texture = 0;
    int unit = 0;
    std::string imagePath {};
};


class YUVTexture: public Texture {
public:
    // Note: yuv texture occupies 3 texture unit
    // TODO: implement single texture unit version
    explicit YUVTexture(int unit1);

    ~YUVTexture() override;

    void init(GLuint currentProgram) override;

    void setParam(GLenum type, GLint value) const override ;

    GLuint getTexture() const override;

    void bind() const override;

    void bufferData(const uint8_t* const* yuvData, int width, int height, ColorFormat fmt);

private:
    int unit1 = 0;
    int width = 0;
    int height = 0;
    ColorFormat format;
    std::vector<GLuint> texture;
};

#endif //GRAPHICS_TEXTURE_H

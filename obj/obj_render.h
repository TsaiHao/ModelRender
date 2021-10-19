#ifndef GRAPHICS_OBJ_RENDER_H
#define GRAPHICS_OBJ_RENDER_H

#include <vector>
#include <memory>
#include "glad/glad.h"

#include "obj_loader.h"

class ObjRender {
public:
    explicit ObjRender(std::shared_ptr<ObjLoader> loader);
    explicit ObjRender(std::string const& objFile);

    void bufferData();
    void draw();

private:
    GLuint VAO, VBO;
    std::shared_ptr<ObjLoader> obj;
    std::vector<float> vertices;        // store vertices' position/texture/normal data
};


#endif //GRAPHICS_OBJ_RENDER_H

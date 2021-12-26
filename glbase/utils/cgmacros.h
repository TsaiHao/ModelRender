#ifndef GRAPHICS_CGMACROS_H
#define GRAPHICS_CGMACROS_H

#define PERFORMANCE Performance __perf__macro(__FUNCTION__)
#define _glCheckError() glCheckError_(__FILE__, __LINE__)

#define GL_WINDOW_WIDTH (800)
#define GL_WINDOW_HEIGHT (800)

#define VPTR(x) (glm::value_ptr(x))

#define FLOAT_EQUAL(x, y) (abs(x - y) < 1e6)

#define LOG_TAG "GLBASE"

#define ROTATE_STATIC_ANGLE "rotate_static_angle"

#endif //GRAPHICS_CGMACROS_H

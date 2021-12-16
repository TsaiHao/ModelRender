#ifndef GRAPHICS_CGMACROS_H
#define GRAPHICS_CGMACROS_H

#define PERFORMANCE Performance __perf__macro(__FUNCTION__)
#define _glCheckError() glCheckError_(__FILE__, __LINE__)

#define GL_WINDOW_WIDTH (800)
#define GL_WINDOW_HEIGHT (800)

#define ROTATE_ANGLE_KEY 0x1

#endif //GRAPHICS_CGMACROS_H

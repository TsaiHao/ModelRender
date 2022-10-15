#ifndef GRAPHICS_CGMACROS_H
#define GRAPHICS_CGMACROS_H

#define PERFORMANCE Performance __perf__macro(__FUNCTION__)
#define _glCheckError() glCheckError_(__FILE__, __LINE__)

#ifdef USE_GLFW
class GLFWwindow;
using WindowType = GLFWwindow*;
#else
using WindowType = void*;
#endif

#define GL_WINDOW_WIDTH (800)
#define GL_WINDOW_HEIGHT (800)

#define VPTR(x) (glm::value_ptr(x))

#define FLOAT_EQUAL(x, y) (abs(x - y) < 1e6)

#define LOG_TAG "GLBASE"

#define ROTATE_STATIC_ANGLE "rotate_static_angle"

// shader related
#define MODEL_UNIFORM 			"model"
#define VIEW_UNIFORM 			"view"
#define PROJECTION_UNIFORM 		"projection"
#define TEXTURE_UNIFORM 		"texture1"
#define TEXTURE_UNIFORM2 		"texture2"
#define TEXTURE_UNIFORM3 		"texture3"
#define MIX_RATION_UNIFORM 		"textureRatio"
#define LIGHT_SOURCE_POSITION 	"lightPos"
#define CAMERA_POSITION 		"cameraPos"
#define PIXEL_FORMAT 			"pixelFormat"
#define USE_TEXTURE				"useTexture"

#endif //GRAPHICS_CGMACROS_H

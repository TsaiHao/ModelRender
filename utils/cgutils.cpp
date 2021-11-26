#include "cgutils.h"

using namespace std;

struct strFinder {
    bool operator()(const char ch) const {
        return (ch <= '9' && ch >= '0') || (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || (ch == '-');
    }
};

static void glfwErrorCallback(int error, const char* description)
{
    Logger::error(description);
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLFWwindow *glWindowInit()
{
    GLFWwindow* window;

    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, "Model Render", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, glfwKeyCallback);

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);

    return window;
}

std::string readTextFile(const std::string &file) {
    std::ifstream ifs(file);
    if (!ifs) {
        Logger::error("open file failed "s + file);
        return "";
    }

    ifs.seekg(0, std::ios::end);
    int size = ifs.tellg();
    std::string buffer(size, 0);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)buffer.data(), size);
    ifs.close();
    
    return buffer;
}

std::string stripString(const std::string& str) {
    strFinder finder;
    auto left = find_if(str.begin(), str.end(), finder);
    auto right = find_if(str.rbegin(), str.rend(), finder);
    if (left == str.end()) {
        return "";
    }
    return string(left, right.base());
}

std::vector<std::string> splitString(const std::string &str, const std::string &del, bool discardSpace /* = false */) {
    vector<string> res;
    if (str.empty()) {
        return res;
    }

    int left = 0;
    int right = str.size() - 1;
    while (1) {
        right = str.find(del, left);
        if (right == string::npos) {
            if (left < str.size()) {
                auto substr = str.substr(left, str.size() - left);
                if (discardSpace) {
                    substr = stripString(substr);
                    if (substr.size() != 0) {
                        res.push_back(substr);
                    }
                }
            }
            break;
        }
        auto substr = str.substr(left, right - left);
        substr = stripString(substr);
        if (substr.size() > 0) {
            res.push_back(substr);
        }
        left = right + del.size();
    }

    return res;
}

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        Logger::error("gl error: %s[%d]: %s", file, line, error.c_str());
    }
    return errorCode;
}

void Logger::error(const string &msg) {
    cerr << "Graphics Error: " << msg << endl;
}

void Logger::warn(const string &msg) {
    cerr << "Graphics Warn: " << msg << endl;
}

void Logger::message(const std::string& msg)
{
    cout << "Graphics Message: " << msg << endl;
}

Performance::Performance(std::string name)
{
    tag = name;
    startTimeUs = glfwGetTime() * 1e6;
}

Performance::~Performance()
{
    endTimeUs = glfwGetTime() * 1e6;
    Logger::message(tag + " consumed time: " + to_string(float(endTimeUs - startTimeUs) / 1000) + " ms");
}

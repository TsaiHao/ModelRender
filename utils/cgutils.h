#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

#include "glad/glad.h"
#include "glfw/glfw3.h"

#define GL_WINDOW_WIDTH (1500)
#define GL_WINDOW_HEIGHT (1500)

#define PERFORMANCE Performance __perf__macro(__FUNCTION__)

void glfwErrorCallback(int error, const char* description);

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow *glWindowInit();

std::string readTextFile(const std::string &file); 

std::string stripString(const std::string& str);

std::vector<std::string> splitString(const std::string &str, const std::string &del, bool discardSpace = true);

template <typename Container1, typename Container2>
void pushVector(Container1& target, const Container2 &source) {
    std::copy(source.begin(), source.end(), std::back_inserter(target));
}

template <typename T>
void printVertices(const std::string &file, const std::vector<T> &vertices, const int lineWidth) {
    std::ofstream ofs(file, std::ios::out);
    for (int i = 0; i < vertices.size(); ++i) {
        ofs << vertices[i] << ", ";
        if (i % lineWidth == lineWidth - 1) {
            ofs << std::endl;
        }
    }
}

GLenum glCheckError_(const char *file, int line);
#define _glCheckError() glCheckError_(__FILE__, __LINE__) 

class Logger {
public:
    static void error(const std::string &msg);
    static void warn(const std::string &msg);
    static void message(const std::string& msg);
};

class Performance {
public:
    explicit Performance(std::string name);
    ~Performance();

private:
    std::string tag;
    int64_t startTimeUs;
    int64_t endTimeUs;
};
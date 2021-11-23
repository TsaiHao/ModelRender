#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define GL_WINDOW_WIDTH (800)
#define GL_WINDOW_HEIGHT (800)

#define PERFORMANCE Performance __perf__macro(__FUNCTION__)

GLenum glCheckError_(const char* file, int line);
#define _glCheckError() glCheckError_(__FILE__, __LINE__) 

GLFWwindow *glWindowInit();

std::string readTextFile(const std::string &file); 

std::string stripString(const std::string& str);

std::vector<std::string> splitString(const std::string &str, const std::string &del, bool discardSpace = true);

template <typename ... Args>
std::string formatString(const std::string& fmt, Args ... args) {
    int len = snprintf(NULL, 0, fmt.c_str(), args ...);
    if (len < 0) {
        return "format string error";
    }
    std::string buffer(len + 1, '\0');
    snprintf(buffer.data(), buffer.size(), fmt.c_str(), args ...);
    return buffer;
}

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
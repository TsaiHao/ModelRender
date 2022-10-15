#pragma once

#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstdio>
#include <assert.h>

#ifndef __ANDROID__
#include "glad/glad.h"
#else
#include "GLES3/gl3.h"
#endif
#include "cgdefs.h"

#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#endif

#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#include "cgdefs.h"

int glCheckError_(const char* file, int line);

double getTime();

WindowType glWindowInit();

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
    std::snprintf((char*)buffer.data(), buffer.size(), fmt.c_str(), args ...);
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

template <typename VectorType1, typename VectorType2>
bool isVectorEqual(const VectorType1& vec1, const VectorType2& vec2, std::size_t vecSize = 0, double eps = 1e-7) {
    if (vecSize == 0) {
        vecSize = vec1.size();
    }
    for (int i = 0; i < vecSize; ++i) {
        if (std::abs(vec1[i] - vec2[i]) > eps) {
            return false;
        }
    }
    return true;
}

template <typename VecType>
auto vec2array(VecType v) {
    typedef typename VecType::value_type T;
    std::array<T, VecType::size> arr;
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = v[i];
    }
    return arr;
}

class Logger {
public:
    static void error(const std::string &msg);
    static void warn(const std::string &msg);
    static void info(const std::string& msg);

    template <typename ... Args>
    static void error(const std::string& fmt, Args ... args) {
        auto msg = formatString(fmt, args...);
        Logger::error(msg);
    }

    template <typename ... Args>
    static void warn(const std::string& fmt, Args ... args) {
        auto msg = formatString(fmt, args...);
        Logger::warn(msg);
    }

    template <typename ... Args>
    static void message(const std::string& fmt, Args ... args) {
        auto msg = formatString(fmt, args...);
        Logger::info(msg);
    }
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

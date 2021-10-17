#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

#include "glad/glad.h"

std::string readTextFile(const std::string &file); 

std::string stripString(const std::string& str);

std::vector<std::string> splitString(const std::string &str, const std::string &del, bool discardSpace = true);

template <typename Container1, typename Container2>
void pushVector(Container1& target, const Container2 &source) {
    std::copy(source.begin(), source.end(), std::back_inserter(target));
}

GLenum glCheckError_(const char *file, int line);
#define _glCheckError() glCheckError_(__FILE__, __LINE__) 

class Logger {
public:
    static void error(const std::string &msg);
    static void warn(const std::string &msg);
};
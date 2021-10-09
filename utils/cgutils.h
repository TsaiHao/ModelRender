#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "glad/glad.h"

std::string readTextFile(const std::string &file); 

std::vector<std::string> splitString(const std::string &str, const std::string &del);

GLenum glCheckError_(const char *file, int line);
#define _glCheckError() glCheckError_(__FILE__, __LINE__) 

class Logger {
public:
    static void error(const std::string &msg);
    static void warn(const std::string &msg);
};
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

std::string readTextFile(const std::string &file); 

std::vector<std::string> splitString(const std::string &str, const std::string &del);

class Logger {
public:
    static void error(const std::string &msg);
    static void warn(const std::string &msg);
};
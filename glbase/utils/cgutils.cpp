#include "cgutils.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif

using namespace std;

struct strFinder {
    bool operator()(const char ch) const {
        return (ch <= '9' && ch >= '0') || (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || (ch == '-');
    }
};



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

int glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
#ifndef __ANDROID__
            //case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            //case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
#endif
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        Logger::error("gl error: %s[%d]: %s", file, line, error.c_str());
    }
    return static_cast<int>(errorCode);
}

double getTime() {
#ifdef USE_GLFW
    return glfwGetTime();
#else
    #ifndef _WIN32
        timeval t;
        gettimeofday(&t, nullptr);
        double ret = t.tv_usec;
        ret /= 1e6;
        ret += t.tv_sec;
        return ret;
    #else
        return 0;
    #endif
#endif
}

void Logger::error(const string &msg) {
#ifndef __ANDROID__
    cerr << "Graphics Error: " << msg << endl;
#else
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", msg.c_str());
#endif
}

void Logger::warn(const string &msg) {
#ifndef __ANDROID__
    cerr << "Graphics Warn: " << msg << endl;
#else
    __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "%s", msg.c_str());
#endif
}

void Logger::message(const std::string& msg)
{
#ifndef __ANDROID__
    cout << "Graphics Message: " << msg << endl;
#else
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", msg.c_str());
#endif
}

Performance::Performance(std::string name)
{
    tag = name;
    startTimeUs = getTime() * 1e6;
}

Performance::~Performance()
{
    endTimeUs = getTime() * 1e6;
    Logger::message(tag + " consumed time: " + to_string(float(endTimeUs - startTimeUs) / 1000) + " ms");
}

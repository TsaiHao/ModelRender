#include "cgutils.h"

using namespace std;

std::string readTextFile(const std::string &file) {
    std::ifstream ifs(file);
    if (!ifs) {
        std::cerr << "Error: open file " << file << " failed" << std::endl;
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

std::vector<std::string> splitString(const std::string &str, const std::string &del) {
    vector<string> res;
    if (str.empty()) {
        return res;
    }

    int left = 0;
    int right = str.size() - 1;
    do {
        right = str.find(del, left);
        if (right == string::npos) {
            if (left < str.size()) {
                res.push_back(str.substr(left, str.size() - left));
            }
            break;
        }
        res.push_back(str.substr(left, right - left));
        left = right + del.size();
    } while (1);

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
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

void Logger::error(const string &msg) {
    cerr << "Error " << msg << endl;
}

void Logger::warn(const string &msg) {
    cerr << "Warn " << msg << endl;
}
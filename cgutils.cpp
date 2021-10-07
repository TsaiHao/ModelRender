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
            if (left < str.size() - 1) {
                res.push_back(str.substr(left, str.size() - left));
            }
            break;
        }
        res.push_back(str.substr(left, right - left));
        left = right + del.size();
    } while (1);

    return res;
}

void Logger::error(const string &msg) {
    cerr << "Error " << msg << endl;
}

void Logger::warn(const string &msg) {
    cerr << "Warn " << msg << endl;
}
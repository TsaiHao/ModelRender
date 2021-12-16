#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <array>
#include <inttypes.h>

#include "../glbase/glbase.h"

#define TEST_PRINT cout << "-----------------TEST-----------------" << endl; \
                   cout << "test function " << __FUNCTION__ << endl;

using namespace std;

void testSingleStringSplit(const string &str, const string &del) {
    auto res = splitString(str, del, true);
    cout << "test string: " << str << endl;
    for (auto const& r : res) {
        cout << " substr: " << r << ", size: " << r.size() << endl;
    }
    cout << endl;
}

void testStringSpilt() {
    TEST_PRINT

    testSingleStringSplit("this  is a test string\n", " ");

    testSingleStringSplit("this is a test string ", " ");

    ifstream ifs("../resource/cube.obj");
    string line;
    for (int i = 0; i < 10; i++) {
        getline(ifs, line);
    }
    ifs.close();
    testSingleStringSplit(line, " ");

    testSingleStringSplit("this/is/a/test//line", "/");

    testSingleStringSplit("thisisateststring", " ");
}

void testObjLoader(const std::string &objFile) {
    TEST_PRINT

    ObjLoader obj(objFile);

    cout << "obj vertices get " << obj.vertices.size() << endl;
    cout << "vertices list: " << endl;
    for (int i = 0; i < obj.vertices.size(); ++i) {
        auto const& vert = obj.vertices[i];
        cout << " vertex " << i << ": ";
        for (int j = 0; j < 4; ++j) {
            cout << vert.point[j] << " ";
        }
        cout << endl;
    }    
}

void testPushVector() {
    vector<int> nums{1, 2, 3};
    array<int, 3> nums2{4, 5, 6};
    pushVector(nums, nums2);

    TEST_PRINT
    cout << "nums : " << endl;
    for (auto const &n : nums) {
        cout << n << '\t';
    }
    cout << endl;
}

void testFormatString() {
    PERFORMANCE;
    string fmt("string: %s;\ninteger: %5d\nfloat: %.4f");
    string str("test string");
    int i = 1234;
    float f = 3.1415926f;

    TEST_PRINT;
    cout << formatString(fmt, str.c_str(), i, f);
}

void testLogger() {
    PERFORMANCE;
    Logger::error("test error: %d, %f", 123, 3.124);
    Logger::warn("test warn: %" PRIu64 " %.4f", 12314UL, 1.234);
    Logger::message("test message: %s", __FUNCTION__);
}

int main(int argc, char **argv) {
    return 0;
}

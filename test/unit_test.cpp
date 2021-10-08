#include <iostream>
#include <vector>
#include <iterator>
#include <string>

#include "obj_loader.h"
#include "cgutils.h"

using namespace std;

void testSingleStringSplit(const string &str, const string &del) {
    cout << "----------------TEST------------------" << endl;
    cout << "stringSplit\n input: " << str << endl << " delimiter: " << del << endl << endl;

    auto res = splitString(str, del);
    for (auto const& r : res) {
        cout << " substr: " << r << ", size: " << r.size() << endl;
    }
}

void testStringSpilt() {
    testSingleStringSplit("this is a test string\n", " ");

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
    cout << "----------------TEST------------------" << endl;
    cout << "test obj file loader file: " << objFile << endl;

    ObjLoader obj(objFile);

    cout << "obj vertices get " << obj.vertices.size() << endl;
    cout << "vertices list: " << endl;
    for (int i = 0; i < obj.vertices.size(); ++i) {
        auto const& vert = static_pointer_cast<ObjVertex>(obj.vertices[i]);
        cout << " vertex " << i << ": ";
        for (int j = 0; j < 4; ++j) {
            cout << vert->point[j] << " ";
        }
        cout << endl;
    }    
}

int main(int argc, char **argv) {
    testObjLoader("../resource/cube.obj");

    float t[] = {1, 2, 3};
    cout << sizeof(t) << endl;
    return 0;
}

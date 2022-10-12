#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

#include "shader.h"
#include "cgutils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glm/glm.hpp"

using namespace std;

static void addVertexShaderMacros(const Shader& shader, const string& source) {

}

static void addFragmentShaderMacros(const Shader& shader, const string& source) {

}

static void checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            stringstream ss;
            ss << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
               << "\n -- --------------------------------------------------- -- " << std::endl;
            Logger::error(ss.str());
            abort();
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            stringstream ss;
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            ss << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
               << "\n -- --------------------------------------------------- -- " << std::endl;
            Logger::error(ss.str());
            abort();
        }
    }
}

Shader::Shader(string vertFile, string fragFile):
    vertSourceFile(std::move(vertFile)), fragSourceFile(std::move(fragFile))
{
    init();
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
    _glCheckError();
}

GLint Shader::getUniformLocation(const std::string &name) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    return loc;
}

Shader::Shader(const Shader &shd) {
    program = shd.program;
    vertShader = shd.vertShader;
    fragShader = shd.fragShader;
}

void Shader::setMat4(const string &name, const float *value) const {
    use();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

void Shader::setFloatVec4(const string &name, const float * const value) const {
    use();
    glUniform4fv(getUniformLocation(name), 1, value);
    _glCheckError();
}

void Shader::setInt(const string &name, int value) const {
    use();
    glUniform1i(getUniformLocation(name), value);
}

void Shader::modifyShaderVersion(std::string &shader) {
    if (shader.empty()) {
        return;
    }

    stringstream ss(shader);
    string line;
    getline(ss, line);

    if (line.substr(0, 8) == "#version") {
        return;     // use shader version
    }

#if defined(__ANDROID__) || defined(__IOS__)
    shader = string("#version 300 es\n") + shader;
#else
    shader = string("#version 330 core\n") + shader;
#endif
}

std::vector<std::string> Shader::getAllUniformList() const {
    vector<string> names;
    GLsizei size = 100, len = 0;
    GLint sz = 0;
    GLenum en;
    GLchar buffer[100];

    GLint count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

    for (GLuint i = 0; i < count; ++i) {
        glGetActiveUniform(program, i, size, &len, &sz, &en, buffer);
        names.emplace_back(string(buffer, buffer + len));
    }

    return names;
}

void Shader::setModelMatrix(const float *const mat) const {
    setMat4(MODEL_UNIFORM, mat);
}

void Shader::setViewMatrix(const float *mat) const {
    setMat4(VIEW_UNIFORM, mat);
}

void Shader::setProjectionMatrix(const float *mat) const {
    setMat4(PROJECTION_UNIFORM, mat);
}

void Shader::init() {
    string vertStr = readTextFile(vertSourceFile);
    string fragStr = readTextFile(fragSourceFile);
    addVertexShaderMacros(*this, vertStr);
    addFragmentShaderMacros(*this, fragStr);
    modifyShaderVersion(vertStr);
    modifyShaderVersion(fragStr);
    assert(!vertStr.empty() && !fragStr.empty());

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertChars = vertStr.c_str();
    glShaderSource(vertShader, 1, &(vertChars), NULL);
    glCompileShader(vertShader);
    checkCompileErrors(vertShader, "VERTEX");

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragChars = fragStr.c_str();
    glShaderSource(fragShader, 1, &(fragChars), NULL);
    glCompileShader(fragShader);
    checkCompileErrors(fragShader, "FRAGMENT");

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");
    _glCheckError();

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    setFloat(MIX_RATION_UNIFORM, 0);
}

void Shader::updateShaders(const std::string& vf, const std::string& ff) {
    vertSourceFile = vf;
    fragSourceFile = ff;
    init();
}

void Shader::setFloat(const string &name, float value) const {
    use();
    glUniform1f(getUniformLocation(name), value);
}

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "shader.h"
#include "cgutils.h"

using namespace std;

Shader::Shader(const string &vertFile, const string &fragFile) {
    string vertStr = readTextFile(vertFile);
    string fragStr = readTextFile(fragFile);
    assert(!vertStr.empty() && !fragStr.empty());

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertChars = vertStr.c_str();
    glShaderSource(vertShader, 1, &(vertChars), NULL);
    glCompileShader(vertShader);
    checkCompileErrors(vertShader, "VERTEX");

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragChars = fragStr.c_str();
    glShaderSource(fragShader, 1, &(fragChars), NULL);
    glCompileShader(fragShader);
    checkCompileErrors(fragShader, "FRAGMENT");
    
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) const
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

GLint Shader::getUniformLocation(const std::string &name) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc < 0) {
        cerr << "Error: get uniform location error: name: " << name << " loc: " << loc << endl;
    }

    return loc;
}
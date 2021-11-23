#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "shader.h"
#include "cgutils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    _glCheckError();
}

Shader::~Shader() {
    //glDeleteProgram(program);
}

void Shader::use(const bool bindTextures) const
{
    _glCheckError();

    glUseProgram(program);
    if (bindTextures) {
        for (int i = 0; i < textures.size(); ++i) {
            textures[i].bind(i);
        }
    }
    _glCheckError();
}

void Shader::attachTexture(const std::string& texName, const Texture& tex)
{
    if (textures.size() > 16) {
        return;
    }

    use();
    setInt(texName.c_str(), textures.size());
    textures.emplace_back(tex);
    Logger::message("shader " + to_string(program) + " attach tex " + to_string(tex.getTexture()));
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

Texture::Texture(const std::string& imagePath)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nChannel, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    Logger::message("new texture " + to_string(texture) + ", image " + imagePath);
}

void Texture::setParam(GLenum type, GLint value) const
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, type, value);
}

void Texture::bind(const int unit) const
{
    _glCheckError();
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    _glCheckError();
}

#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Shader {
public:
    Shader() = default;
    Shader(const std::string &vertFile, const std::string &fragFile);
    ~Shader();

    void use() const {
        glUseProgram(program);
    }

    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(getUniformLocation(name), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(getUniformLocation(name), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(getUniformLocation(name), value); 
    }

    void setFloatVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
    }
    void setFloatVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
    }

    void setMat3(const std::string &name, const glm::mat3 &value) const {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }
    void setMat4(const std::string &name, const glm::mat4 &value) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type) const;
    GLint getUniformLocation(const std::string &name) const;

    GLuint vertShader;
    GLuint fragShader;
    GLuint program;
};
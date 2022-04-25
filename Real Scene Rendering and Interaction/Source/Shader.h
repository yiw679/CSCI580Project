#pragma once

#include "glad/glad.h"
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int vs_id;
    unsigned int fs_id;

    unsigned int shaderProgram;

    Shader(const char* vs, const char* fs);

    void Enable();

    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void setVec4(const std::string& name, const GLfloat* value) const;

    void setVec3(const std::string& name, const GLfloat* value) const;

    void setFloat(const std::string& name, float value) const;

    void setInt(const std::string& name, int value) const;

    void checkCompileErrors(GLuint shader, std::string type);
};


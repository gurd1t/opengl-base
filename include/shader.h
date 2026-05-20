#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
public:
    unsigned int programId;

    Shader(const char *vertexPath, const char *fragmentPath) {
        const unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexPath);
        const unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

        programId = glCreateProgram();
        glAttachShader(programId, vShader);
        glAttachShader(programId, fShader);
        glLinkProgram(programId);

        checkCompileErrors(programId, "PROGRAM");
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    // methods
    void use() const {
        glUseProgram(programId);
    }

    void setBoolUniform(const std::string &name, const bool value) const {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), static_cast<int>(value));
    }

    void setIntUniform(const std::string &name, const int value) const {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }

    void setFloatUniform(const std::string &name, const float value) const {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }

    void setVec2Uniform(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
    }
    void setVec2Uniform(const std::string &name, const float x, const float y) const {
        glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
    }

    void setVec3Uniform(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
    }
    void setVec3Uniform(const std::string &name, const float x, const float y, const float z) const {
        glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
    }

    void setVec4Uniform(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
    }
    void setVec4Uniform(const std::string &name, const float x, const float y, const float z, const float w) const {
        glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
    }

    void setMat2Uniform(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3Uniform(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4Uniform(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    static unsigned int compileShader(unsigned int type, const char *shaderPath) {
        std::string shaderCodeStream;
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderCodeStream = shaderStream.str();
        } catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char *shaderCode = shaderCodeStream.c_str();
        const unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, "SHADER");
        return shader;
    }

    static void checkCompileErrors(const unsigned int shader, const std::string_view type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                        "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                        "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};


#endif

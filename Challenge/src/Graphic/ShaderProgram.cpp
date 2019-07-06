#include "ShaderProgram.h"
#include <functional>
#include <memory>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

gl::ShaderProgram::ShaderProgram(const char* szCodeOfVertShader, const char* szCodeOfFragShader) 
    :mProgram(glCreateProgram())
{
    std::unique_ptr<unsigned int, std::function<void(unsigned int*)>>
        pVertexShader(new unsigned int(glCreateShader(GL_VERTEX_SHADER)),
                        [](unsigned int * data) {
                            glDeleteShader(*data);
                            delete data;
                        }),
        pFragmentShader(new unsigned int(glCreateShader(GL_FRAGMENT_SHADER)),
                        [](unsigned int* data) {
                            glDeleteShader(*data);
                            delete data;
                        });
    glShaderSource(*pVertexShader, 1, &szCodeOfVertShader, NULL);
    glShaderSource(*pFragmentShader, 1, &szCodeOfFragShader, NULL);

    char log[512];
    int success, logSize = sizeof(log);

    glCompileShader(*pVertexShader);
    glGetShaderiv(*pVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*pVertexShader, logSize, NULL, log);
        glDeleteProgram(mProgram);
        throw std::runtime_error(std::string("Cannot compile vertex shader: ") + log);
    }

    glCompileShader(*pFragmentShader);
    glGetShaderiv(*pFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*pFragmentShader, logSize, NULL, log);
        glDeleteProgram(mProgram);
        throw std::runtime_error(std::string("Cannot compile fragment shader: ") + log);
    }

    glAttachShader(mProgram, *pVertexShader);
    glAttachShader(mProgram, *pFragmentShader);
    glLinkProgram(mProgram);

    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mProgram, logSize, NULL, log);
        glDeleteProgram(mProgram);
        throw std::runtime_error(std::string("Cannot link shader program: ") + log);
    }
}

gl::ShaderProgram::ShaderProgram(ShaderProgram&& rhs) noexcept
    :mProgram(rhs.mProgram)
{
    rhs.mProgram = 0;
}

gl::ShaderProgram::~ShaderProgram() noexcept {
    glDeleteProgram(mProgram);
}

void gl::ShaderProgram::use() const noexcept {
    glUseProgram(mProgram);
}

void gl::ShaderProgram::disuseCurrent() noexcept {
    glUseProgram(0);
}

void gl::ShaderProgram::setUniform(const std::string& name, float value) noexcept
{
    use();
    glUniform1fv(getUniformLocation(name), 1, &value);
    disuseCurrent();
}

void gl::ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) noexcept
{
    use();
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
    disuseCurrent();
}

void gl::ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) noexcept
{
    use();
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
    disuseCurrent();
}

void gl::ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) noexcept
{
    use();
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
    disuseCurrent();
}

void gl::ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) noexcept
{
    use();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    disuseCurrent();
}

unsigned int gl::ShaderProgram::getUniformLocation(const std::string& name) const noexcept {
    return glGetUniformLocation(mProgram, name.c_str());
}

gl::ShaderProgram gl::loadShaderProgramFromFile(
        const std::string& filenameOfVertShader, 
        const std::string& filenameOfFragShader) {
    std::ifstream 
        fileOfVertShader(filenameOfVertShader),
        fileOfFragShader(filenameOfFragShader);
    std::stringstream sbuffer;
    sbuffer << fileOfVertShader.rdbuf();
    fileOfVertShader.close();
    std::string strSourceOfVertShader = sbuffer.str();

    sbuffer.str("");
    sbuffer << fileOfFragShader.rdbuf();
    fileOfFragShader.close();
    std::string strSourceOfFragShader = sbuffer.str();
    return ShaderProgram(strSourceOfVertShader.c_str(), strSourceOfFragShader.c_str());
}

#pragma once
#include <glm/glm.hpp>
#include <string>
#include <cstdint>
#include "../Util/Noncopyable.h"
#include "../Util/Nonmovable.h"

namespace gl {
    class ShaderProgram
        :private Noncopyable
    {
    public:
        ShaderProgram(const char* szCodeOfVertShader, const char* szCodeOfFragShader);
        ShaderProgram(ShaderProgram&& rhs) noexcept;
        ShaderProgram& operator=(ShaderProgram&&) = delete;
        ~ShaderProgram() noexcept;

        void use() const noexcept;
        static void disuseCurrent() noexcept;

        void setUniform(const std::string& name, float value) noexcept;
        void setUniform(const std::string& name, const glm::vec2& value) noexcept;
        void setUniform(const std::string& name, const glm::vec3&  value) noexcept;
        void setUniform(const std::string& name, const glm::vec4&  value) noexcept;
        void setUniform(const std::string& name, const glm::mat4&  value) noexcept;
    private:
        std::uint32_t mProgram;
        std::uint32_t getUniformLocation(const std::string& name) const noexcept;
    };

    ShaderProgram loadShaderProgramFromFile(
        const std::string& filenameOfVertShader, 
        const std::string& filenameOfFragShader);
}

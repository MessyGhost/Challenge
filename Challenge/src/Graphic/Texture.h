#pragma once
#include "OpenGL.hpp"
#include "../Util/Noncopyable.h"
#include <cstdint>

namespace gl {
    //TODO: fill it
    enum class TextureBindingTarget : GLenum {
        Undefined = 0,
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        Texture2DArray = GL_TEXTURE_2D_ARRAY
    };

    //TODO: fill it
    enum class TextureScaleMode : GLenum {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    class Texture 
        :private Noncopyable
    {
    public:
        Texture(TextureBindingTarget target, 
                std::uint32_t width, std::uint32_t height, std::uint32_t depth = 1, 
                const void* data = NULL) noexcept;
        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) = delete;
        ~Texture() noexcept;
        void bind() const noexcept;
        static void clearTargetBinding(TextureBindingTarget target) noexcept;
        void subData(std::uint32_t x, std::uint32_t y, std::uint32_t z,
                     std::uint32_t w, std::uint32_t h, std::uint32_t d,
                     const void* data) noexcept;
        std::uint32_t width() const noexcept;
        std::uint32_t height() const noexcept;
        std::uint32_t depth() const noexcept;
    private:
        std::uint32_t mTexture;
        const std::uint32_t mWidth, mHeight, mDepth;
        const GLenum mTarget;
    };
}

#pragma once
#include "OpenGL.hpp"
#include "../Util/Noncopyable.h"
#include <cstdint>

namespace gl {
    //TODO: fill it.
    enum class BufferType {
        ColorBuffer = GL_COLOR,
        DepthBuffer = GL_DEPTH
    };

    class Framebuffer
        :private Noncopyable
    {
    public:
        static Framebuffer SCREEN;
        void clearBuffer(BufferType type, const float* data) noexcept;
        ~Framebuffer() noexcept;
        //TODO: implements the methods below.
        Framebuffer() noexcept;
        Framebuffer(Framebuffer&&) noexcept;
        Framebuffer& operator=(Framebuffer&&) = delete;
    private:
        std::uint32_t mFBO;
        Framebuffer(std::uint32_t nativeBuffer) noexcept;
    };
}

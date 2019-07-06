#pragma once
#include "OpenGL.hpp"
#include "../Util/Noncopyable.h"
#include <initializer_list>
#include <vector>
#include <utility>

namespace gl {
    enum class BufferUsage : GLenum {
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY
    };

    enum class BufferBindingTarget : GLenum {
        ArrayBuffer = GL_ARRAY_BUFFER,
        ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
        PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
        PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
        TextureBuffer = GL_TEXTURE_BUFFER
    };

    class VertexArray;

    class VertexBuffer 
        :private Noncopyable
    {
    public:
        VertexBuffer(unsigned long size, BufferUsage usage, const void* data = (void*)0) noexcept;
        VertexBuffer(VertexBuffer&& rhs) noexcept;
        VertexBuffer& operator=(VertexBuffer&&) = delete;
        ~VertexBuffer() noexcept;

        unsigned long size() const noexcept;
        void subData(unsigned long size, const void* data, unsigned long offset = 0) noexcept;
    private:
        friend class VertexArray;
        unsigned int mVBO;
        unsigned long mSize;
        void bind(BufferBindingTarget target) const noexcept;
        static void clearTargetBinding(BufferBindingTarget target) noexcept;
    };

    template<typename T>
    inline VertexBuffer createVertexBuffer(const std::initializer_list<T>& data, BufferUsage usage) noexcept {
        std::vector<T> vec = std::move(data);
        return VertexBuffer(data.size() * sizeof(T), usage, vec.data());
    }
}

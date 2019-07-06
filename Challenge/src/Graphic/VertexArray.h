#pragma once
#include "VertexBuffer.h"

namespace gl {
    class VertexArray 
        :private Noncopyable
    {
    public:
        VertexArray() noexcept;
        VertexArray(VertexArray&& rhs) noexcept;
        VertexArray& operator=(VertexArray&&) = delete;
        ~VertexArray() noexcept;

        void bind() const noexcept;
        static void unbindCurrent() noexcept;
        void setAttribute(
            const VertexBuffer& buffer,
            unsigned int index,
            unsigned int size,
            DataType type,
            bool normalized = false,
            unsigned int stride = 0,
            const void* offset = (void*)0
        ) noexcept;
        void enableAttribute(unsigned int index) noexcept;
    private:
        unsigned int mVAO;
    };
}

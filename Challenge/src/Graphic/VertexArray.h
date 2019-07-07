#pragma once
#include "VertexBuffer.h"
#include <cstdint>

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
            std::uint32_t index,
            std::int32_t size,
            DataType type,
            bool normalized = false,
            std::uint32_t stride = 0,
            const void* offset = (void*)0
        ) noexcept;
        void enableAttribute(std::uint32_t index) noexcept;
    private:
        std::uint32_t mVAO;
    };
}

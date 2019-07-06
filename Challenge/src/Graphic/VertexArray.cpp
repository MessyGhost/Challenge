#include "VertexArray.h"

gl::VertexArray::VertexArray() noexcept {
    glGenVertexArrays(1, &mVAO);
}

gl::VertexArray::VertexArray(VertexArray&& rhs) noexcept
    :mVAO(rhs.mVAO)
{
    rhs.mVAO = 0;
}

gl::VertexArray::~VertexArray() noexcept {
    glDeleteVertexArrays(1, &mVAO);
}

void gl::VertexArray::bind() const noexcept {
    glBindVertexArray(mVAO);
}

void gl::VertexArray::unbindCurrent() noexcept {
    glBindVertexArray(0);
}

void gl::VertexArray::setAttribute(
            const VertexBuffer& buffer,
            unsigned int index,
            unsigned int size,
            DataType type,
            bool normalized,
            unsigned int stride,
            const void* offset
        ) noexcept
{
    bind();
    buffer.bind(gl::BufferBindingTarget::ArrayBuffer);
    glVertexAttribPointer(index, size, static_cast<GLenum>(type), 
                normalized ? GL_TRUE : GL_FALSE, stride, offset);
    VertexBuffer::clearTargetBinding(gl::BufferBindingTarget::ArrayBuffer);
    unbindCurrent();
}

void gl::VertexArray::enableAttribute(unsigned int index) noexcept {
    bind();
    glEnableVertexAttribArray(0);
    unbindCurrent();
}

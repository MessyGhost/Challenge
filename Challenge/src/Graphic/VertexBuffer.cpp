#include "VertexBuffer.h"

gl::VertexBuffer::VertexBuffer(unsigned long size, gl::BufferUsage usage, const void* data) noexcept
    :mSize(size)
{
    glGenBuffers(1, &mVBO);
    bind(gl::BufferBindingTarget::ArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
    clearTargetBinding(gl::BufferBindingTarget::ArrayBuffer);
}

gl::VertexBuffer::VertexBuffer(gl::VertexBuffer&& rhs) noexcept
    :mVBO(rhs.mVBO), mSize(rhs.mSize)
{
    rhs.mVBO = 0;
}

gl::VertexBuffer::~VertexBuffer() noexcept {
    glDeleteBuffers(1, &mVBO);
}

unsigned long gl::VertexBuffer::size() const noexcept {
    return mSize;
}

void gl::VertexBuffer::subData(unsigned long size, const void* data, unsigned long offset) noexcept {
    bind(gl::BufferBindingTarget::ArrayBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    clearTargetBinding(gl::BufferBindingTarget::ArrayBuffer);
}

void gl::VertexBuffer::bind(gl::BufferBindingTarget target) const noexcept {
    glBindBuffer(static_cast<GLenum>(target), mVBO);
}

void gl::VertexBuffer::clearTargetBinding(gl::BufferBindingTarget target) noexcept {
    glBindBuffer(static_cast<GLenum>(target), 0);
}

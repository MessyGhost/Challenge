#include "Framebuffer.h"

gl::Framebuffer gl::Framebuffer::SCREEN(0);

gl::Framebuffer::Framebuffer(std::uint32_t native) noexcept
    :mFBO(native)
{

}

void gl::Framebuffer::clearBuffer(gl::BufferType type, const float* data) noexcept {
    glClearBufferfv(static_cast<GLenum>(type), mFBO, data);
}

gl::Framebuffer::~Framebuffer() noexcept {
    glDeleteFramebuffers(1, &mFBO);
}

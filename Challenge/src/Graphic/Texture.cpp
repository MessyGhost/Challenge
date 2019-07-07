#include "Texture.h"

gl::Texture::Texture(
    gl::TextureBindingTarget target,
    std::uint32_t width, 
    std::uint32_t height, 
    std::uint32_t depth, 
    const void* data
    ) noexcept
    :mWidth(width), mHeight(height), mDepth(depth), mTarget(static_cast<GLenum>(target))
{
    glGenTextures(1, &mTexture);
    bind();
    glTexImage3D(mTarget, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    clearTargetBinding(static_cast<gl::TextureBindingTarget>(mTarget));
}

gl::Texture::Texture(gl::Texture&& rhs) noexcept
    :mTexture(rhs.mTexture), mWidth(rhs.mWidth), mHeight(rhs.mHeight), mDepth(rhs.mDepth), mTarget(rhs.mTarget)
{
    rhs.mTexture = 0;
}

gl::Texture::~Texture() noexcept {
    glDeleteTextures(1, &mTexture);
}

void gl::Texture::bind() const noexcept {
    glBindTexture(mTarget, mTexture);
}

void gl::Texture::clearTargetBinding(gl::TextureBindingTarget target) noexcept {
    glBindTexture(static_cast<GLenum>(target), 0);
}

void gl::Texture::subData(std::uint32_t x, std::uint32_t y, std::uint32_t z,
                     std::uint32_t w, std::uint32_t h, std::uint32_t d,
                     const void* data) noexcept
{
    bind();
    glTexSubImage3D(mTarget, 0, x, y, z, w, h, d, GL_RGBA, GL_UNSIGNED_BYTE, data);
    clearTargetBinding(static_cast<gl::TextureBindingTarget>(mTarget));
}

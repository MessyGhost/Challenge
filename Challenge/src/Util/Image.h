#pragma once
#include "Noncopyable.h"
#include <cstdint>
#include <string>
#include <functional>

class Image
    :private Noncopyable
{
public:
    Image(std::uint32_t width, std::uint32_t height, std::uint8_t* data, 
		std::function<void(std::uint8_t*)> deleter) noexcept;
    ~Image();
    Image(Image&&) noexcept;
    Image& operator=(Image&&) = delete;
    std::uint32_t width() const noexcept;
    std::uint32_t height() const noexcept;
    const std::uint8_t* data() const noexcept;
    static Image fromFile(const std::string& filename);
private:
    std::function<void(std::uint8_t*)> mDeleter;
    std::uint32_t mWidth, mHeight;
    std::uint8_t* mData;
};

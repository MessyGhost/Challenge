#include "Image.h"
#include <SOIL.h>
#include <stdexcept>

Image::~Image() noexcept {
    if (mData != nullptr)mDeleter(mData);
}

Image::Image(std::uint32_t width, std::uint32_t height, std::uint8_t* data, 
    std::function<void(std::uint8_t*) noexcept> deleter) noexcept
	:mData(data), mWidth(width), mHeight(height), mDeleter(deleter)
{
	
}

Image::Image(Image&& rhs) noexcept
    :mData(rhs.mData), mWidth(rhs.mWidth), mHeight(rhs.mHeight)
{
    rhs.mData = nullptr;
}

std::uint32_t Image::width() const noexcept {
    return mWidth;
}

std::uint32_t Image::height() const noexcept {
    return mHeight;
}

const std::uint8_t* Image::data() const noexcept {
    return mData;
}

Image Image::fromFile(const std::string& filename) {
    std::int32_t width, height, channels;
    std::uint8_t 
        *pixels = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
    if(!pixels) {
        throw std::runtime_error(std::string("Cannot load image from file.\nFilename: ") + filename.c_str());
    }
    return Image(width, height, pixels, 
        [](std::uint8_t *data) noexcept {
            SOIL_free_image_data(data);
        });
}

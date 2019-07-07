#include "Texture2DArrayBuilder.h"
#include "Image.h"
#include <cmath>

gl::Texture Texture2DArrayBuilder::build(std::uint32_t width, std::uint32_t height) const noexcept {
    gl::Texture result(gl::TextureBindingTarget::Texture2DArray, width, height, mImages.size());
    int i = 0;
    for (const Image& image : mImages) {
        if (image.width() != width && image.height() != height) {
            result.subData(0, 0, i++, width, height, 1, scaleImage(image, width, height).data());
        }
        else {
            result.subData(0, 0, i++, width, height, 1, image.data());
        }
    }
    return result;
}

std::uint32_t Texture2DArrayBuilder::append(Image&& image) noexcept {
    std::uint32_t result = mImages.size();
    mImages.push_back(std::move(image));
    return result;
}

Image Texture2DArrayBuilder::scaleImage(const Image& image, std::uint32_t w, std::uint32_t h) noexcept {
    std::uint8_t* resultData = new std::uint8_t[w * h];

    for (std::uint32_t x = 0; x < w; ++x) for (std::uint32_t y = 0; y < h; ++y) {
        std::uint32_t 
        srcX = std::round(x / w * (float)image.width()),
        srcY = std::round(y / h * (float)image.height());
        resultData[srcY * w + srcX] = image.data()[y * image.width() + x];
    }
    
    return Image(w, h, resultData, 
        [](uint8_t* data) noexcept {
            delete data;
        });
}
